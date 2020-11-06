/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "vulkanrenderer.h"

#include <iostream>

#include <QVulkanFunctions>
#include <QCoreApplication>
#include <QFile>
#include <QVulkanFunctions>
#include <QMouseEvent>
#include <QVulkanWindowRenderer>

#include <OpenImageIO/imagebufalgo.h>


#include "vulkanwindow.h"
#include "fileboxentity.h"

// Use a triangle strip to get a quad.
static float vertexData[] = { // Y up, front = CW
    // x, y, z, u, v
    -1,  -1, 0, 0, 1,
    -1,   1, 0, 0, 0,
     1,  -1, 0, 1, 1,
     1,   1, 0, 1, 0
};

static const int UNIFORM_DATA_SIZE = 16 * sizeof(float);

static inline VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}

VulkanRenderer::VulkanRenderer(VulkanWindow *w)
    : window(w)
{
    concurrentFrameCount = window->concurrentFrameCount();
}

void VulkanRenderer::initResources()
{
    device = window->device();
    physicalDevice = window->physicalDevice();

    devFuncs = window->vulkanInstance()->deviceFunctions(device);
    f = window->vulkanInstance()->functions();

    createVertexBuffer();
    createSampler();
    createDescriptorPool();
    createGraphicsDescriptors();
    createGraphicsPipelineCache();
    createGraphicsPipelineLayout();

    // Graphics pipelines
    VkShaderModule fragShader = createShaderFromFile(":/shaders/texture_frag.spv");
    createGraphicsPipeline(graphicsPipelineRGB, fragShader);
    fragShader = createShaderFromFile(":/shaders/texture_alpha_frag.spv");
    createGraphicsPipeline(graphicsPipelineAlpha, fragShader);

    createComputeDescriptors();
    createComputePipelineLayout();

    // Load all the shaders we need
    loadShadersFromDisk();
    // Create Noop pipeline
    noopPipeline = createComputePipelineNoop();
    // Create a pipeline for each shader
    createComputePipelines();

    createComputeQueue();
    createComputeCommandPool();

    try
    {
        const char* file = "ocio/config.ocio";
        ocioConfig = OCIO::Config::CreateFromFile(file);
    }
    catch(OCIO::Exception& exception)
    {
        qDebug("OpenColorIO Error: ");
        qDebug(exception.what());
    }

    emit window->rendererHasBeenCreated();
}

int VulkanRenderer::getDeviceMemorySize()
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    window->vulkanInstance()->functions()->vkGetPhysicalDeviceMemoryProperties(
                window->physicalDevice(), &memoryProperties);

    // TODO: this ^^^

    return 0;
}

QString VulkanRenderer::getGpuName()
{
    VkPhysicalDeviceProperties deviceProperties = {};
    f->vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    auto deviceName = QString::fromLatin1(deviceProperties.deviceName);

    return deviceName;
}

void VulkanRenderer::createVertexBuffer()
{
    const VkPhysicalDeviceLimits *pdevLimits = &window->physicalDeviceProperties()->limits;
    const VkDeviceSize uniAlign = pdevLimits->minUniformBufferOffsetAlignment;
    qDebug("uniform buffer offset alignment is %u", (uint) uniAlign);
    VkBufferCreateInfo bufInfo;
    memset(&bufInfo, 0, sizeof(bufInfo));
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    // Our internal layout is vertex, uniform, uniform, ... with each uniform buffer start offset aligned to uniAlign.
    const VkDeviceSize vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
    const VkDeviceSize uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);
    bufInfo.size = vertexAllocSize + concurrentFrameCount * uniformAllocSize;
    bufInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VkResult err = devFuncs->vkCreateBuffer(device, &bufInfo, nullptr, &buf);
    if (err != VK_SUCCESS)
        qFatal("Failed to create buffer: %d", err);

    VkMemoryRequirements memReq;
    devFuncs->vkGetBufferMemoryRequirements(device, buf, &memReq);

    VkMemoryAllocateInfo memAllocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memReq.size,
        window->hostVisibleMemoryIndex()
    };

    err = devFuncs->vkAllocateMemory(device, &memAllocInfo, nullptr, &bufMem);
    if (err != VK_SUCCESS)
        qFatal("Failed to allocate memory: %d", err);

    err = devFuncs->vkBindBufferMemory(device, buf, bufMem, 0);
    if (err != VK_SUCCESS)
        qFatal("Failed to bind buffer memory: %d", err);

    quint8 *p;
    err = devFuncs->vkMapMemory(device, bufMem, 0, memReq.size, 0, reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS)
        qFatal("Failed to map memory: %d", err);
    memcpy(p, vertexData, sizeof(vertexData));
    QMatrix4x4 ident;
    memset(uniformBufInfo, 0, sizeof(uniformBufInfo));
    for (int i = 0; i < concurrentFrameCount; ++i) {
        const VkDeviceSize offset = vertexAllocSize + i * uniformAllocSize;
        memcpy(p + offset, ident.constData(), 16 * sizeof(float));
        uniformBufInfo[i].buffer = buf;
        uniformBufInfo[i].offset = offset;
        uniformBufInfo[i].range = uniformAllocSize;
    }
    devFuncs->vkUnmapMemory(device, bufMem);
}

void VulkanRenderer::createSampler()
{
    // Create sampler
    VkSamplerCreateInfo samplerInfo;
    memset(&samplerInfo, 0, sizeof(samplerInfo));
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_NEAREST;
    samplerInfo.minFilter = VK_FILTER_NEAREST;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    VkResult err = devFuncs->vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
    if (err != VK_SUCCESS)
        qFatal("Failed to create sampler: %d", err);
}

void VulkanRenderer::createDescriptorPool()
{
    // Create descriptor pool
    VkDescriptorPoolSize descPoolSizes[4] = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 * uint32_t(concurrentFrameCount) },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 * uint32_t(concurrentFrameCount) },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 * uint32_t(concurrentFrameCount) },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          2 * uint32_t(concurrentFrameCount) } //two per frame
    };
    VkDescriptorPoolCreateInfo descPoolInfo;
    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descPoolInfo.maxSets        = 6;
    descPoolInfo.poolSizeCount  = 4;
    descPoolInfo.pPoolSizes = descPoolSizes;
    VkResult err = devFuncs->vkCreateDescriptorPool(
                device,
                &descPoolInfo,
                nullptr,
                &descriptorPool);
    if (err != VK_SUCCESS)
        qFatal("Failed to create descriptor pool: %d", err);
}

void VulkanRenderer::createGraphicsDescriptors()
{
    // Create DescriptorSetLayout
    VkDescriptorSetLayoutBinding layoutBinding[2] =
    {
        {
            0, // binding
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            1, // descriptorCount
            VK_SHADER_STAGE_VERTEX_BIT,
            nullptr
        },
        {
            1, // binding
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            1, // descriptorCount
            VK_SHADER_STAGE_FRAGMENT_BIT,
            nullptr
        }
    };
    VkDescriptorSetLayoutCreateInfo descLayoutInfo = {
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        nullptr,
        0,
        2, // bindingCount
        layoutBinding
    };
    VkResult err = devFuncs->vkCreateDescriptorSetLayout(device, &descLayoutInfo, nullptr, &graphicsDescriptorSetLayout);
    if (err != VK_SUCCESS)
        qFatal("Failed to create descriptor set layout: %d", err);

}

void VulkanRenderer::createGraphicsPipelineCache()
{
    // Pipeline cache
    VkPipelineCacheCreateInfo pipelineCacheInfo;
    memset(&pipelineCacheInfo, 0, sizeof(pipelineCacheInfo));
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VkResult err = devFuncs->vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr, &pipelineCache);
    if (err != VK_SUCCESS)
        qFatal("Failed to create pipeline cache: %d", err);
}

void VulkanRenderer::createGraphicsPipelineLayout()
{
    VkPushConstantRange pushConstantRange;
    pushConstantRange.stageFlags                    = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset                        = 0;
    pushConstantRange.size                          = sizeof(viewerPushConstants);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
    pipelineLayoutInfo.sType =                  VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount =         1;
    pipelineLayoutInfo.pSetLayouts =            &graphicsDescriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges =    &pushConstantRange;

    VkResult err = devFuncs->vkCreatePipelineLayout(
                device,
                &pipelineLayoutInfo,
                nullptr,
                &pipelineLayout);
    if (err != VK_SUCCESS)
        qFatal("Failed to create pipeline layout: %d", err);
}

void VulkanRenderer::createGraphicsPipeline(
        VkPipeline& pl, const VkShaderModule& fragShaderModule)
{
    // Vertex shader never changes
    VkShaderModule vertShaderModule = createShaderFromFile(":/shaders/texture_vert.spv");

    // Graphics pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo;
    memset(&pipelineInfo, 0, sizeof(pipelineInfo));
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    VkPipelineShaderStageCreateInfo shaderStages[2] = {
        {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            VK_SHADER_STAGE_VERTEX_BIT,
            vertShaderModule,
            "main",
            nullptr
        },
        {
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            VK_SHADER_STAGE_FRAGMENT_BIT,
            fragShaderModule,
            "main",
            nullptr
        }
    };

    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    // Vertex binding
    VkVertexInputBindingDescription vertexBindingDesc = {
        0, // binding
        5 * sizeof(float),
        VK_VERTEX_INPUT_RATE_VERTEX
    };
    VkVertexInputAttributeDescription vertexAttrDesc[] = {
        { // position
            0, // location
            0, // binding
            VK_FORMAT_R32G32B32_SFLOAT,
            0
        },
        { // texcoord
            1,
            0,
            VK_FORMAT_R32G32_SFLOAT,
            3 * sizeof(float)
        }
    };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.flags = 0;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &vertexBindingDesc;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexAttributeDescriptions = vertexAttrDesc;

    pipelineInfo.pVertexInputState = &vertexInputInfo;

    VkPipelineInputAssemblyStateCreateInfo ia;
    memset(&ia, 0, sizeof(ia));
    ia.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    ia.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    pipelineInfo.pInputAssemblyState = &ia;

    // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
    // This way the pipeline does not need to be touched when resizing the window.
    VkPipelineViewportStateCreateInfo vp;
    memset(&vp, 0, sizeof(vp));
    vp.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vp.viewportCount = 1;
    vp.scissorCount = 1;
    pipelineInfo.pViewportState = &vp;

    VkPipelineRasterizationStateCreateInfo rs;
    memset(&rs, 0, sizeof(rs));
    rs.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rs.polygonMode = VK_POLYGON_MODE_FILL;
    rs.cullMode = VK_CULL_MODE_BACK_BIT;
    rs.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rs.lineWidth = 1.0f;
    pipelineInfo.pRasterizationState = &rs;

    VkPipelineMultisampleStateCreateInfo ms;
    memset(&ms, 0, sizeof(ms));
    ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineInfo.pMultisampleState = &ms;

    VkPipelineDepthStencilStateCreateInfo ds;
    memset(&ds, 0, sizeof(ds));
    ds.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    ds.depthTestEnable = VK_TRUE;
    ds.depthWriteEnable = VK_TRUE;
    ds.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    pipelineInfo.pDepthStencilState = &ds;

    VkPipelineColorBlendStateCreateInfo cb;
    memset(&cb, 0, sizeof(cb));
    cb.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // assume pre-multiplied alpha, blend, write out all of rgba
    VkPipelineColorBlendAttachmentState att;
    memset(&att, 0, sizeof(att));
    att.colorWriteMask = 0xF;
    att.blendEnable = VK_TRUE;
    att.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    att.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    att.colorBlendOp = VK_BLEND_OP_ADD;
    att.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    att.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    att.alphaBlendOp = VK_BLEND_OP_ADD;
    cb.attachmentCount = 1;
    cb.pAttachments = &att;
    pipelineInfo.pColorBlendState = &cb;

    VkDynamicState dynEnable[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    VkPipelineDynamicStateCreateInfo dyn;
    memset(&dyn, 0, sizeof(dyn));
    dyn.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyn.dynamicStateCount = sizeof(dynEnable) / sizeof(VkDynamicState);
    dyn.pDynamicStates = dynEnable;
    pipelineInfo.pDynamicState = &dyn;

    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = window->defaultRenderPass();

    VkResult err = devFuncs->vkCreateGraphicsPipelines(
                device, pipelineCache, 1, &pipelineInfo, nullptr, &pl);
    if (err != VK_SUCCESS)
        qFatal("Failed to create graphics pipeline: %d", err);

    if (vertShaderModule)
        devFuncs->vkDestroyShaderModule(device, vertShaderModule, nullptr);
    if (fragShaderModule)
        devFuncs->vkDestroyShaderModule(device, fragShaderModule, nullptr);
}

VkShaderModule VulkanRenderer::createShaderFromFile(const QString &name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to read shader %s", qPrintable(name));
        return VK_NULL_HANDLE;
    }
    QByteArray blob = file.readAll();
    file.close();

    VkShaderModuleCreateInfo shaderInfo;
    memset(&shaderInfo, 0, sizeof(shaderInfo));
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = blob.size();
    shaderInfo.pCode = reinterpret_cast<const uint32_t *>(blob.constData());
    VkShaderModule shaderModule;
    VkResult err = devFuncs->vkCreateShaderModule(window->device(), &shaderInfo, nullptr, &shaderModule);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create shader module: %d", err);
        return VK_NULL_HANDLE;
    }

    return shaderModule;
}

void VulkanRenderer::loadShadersFromDisk()
{
    for (int i = 0; i != NODE_TYPE_MAX; i++)
    {
        NodeType nodeType = static_cast<NodeType>(i);

        auto props = getPropertiesForType(nodeType);

        shaders[nodeType] = createShaderFromFile(props.shaderPath);
    }
}

bool VulkanRenderer::createComputeRenderTarget(uint32_t width, uint32_t height)
{
    computeRenderTarget = std::unique_ptr<CsImage>(
                new CsImage(window, &device, devFuncs, width, height));

    emit window->renderTargetHasBeenCreated(width, height);

    currentRenderSize = QSize(width, height);

    return true;
}

bool VulkanRenderer::createTextureFromFile(const QString &path, const int colorSpace)
{
    cpuImage = std::unique_ptr<ImageBuf>(new ImageBuf(path.toStdString()));
    bool ok = cpuImage->read(0, 0, 0, 3, true, TypeDesc::FLOAT);
    if (!ok)
    {
        std::cout << "There was a problem reading the image." << std::endl;
        std::cout << cpuImage->geterror() << std::endl;
    }

    // Add alpha channel if it doesn't exist
    if (cpuImage->nchannels() == 3)
    {
        int channelorder[] = { 0, 1, 2, -1 /*use a float value*/ };
        float channelvalues[] = { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 };
        std::string channelnames[] = { "R", "G", "B", "A" };

        *cpuImage = ImageBufAlgo::channels(*cpuImage, 4, channelorder, channelvalues, channelnames);
    }

    // Transform color space
    //
    // 0 = sRGB
    // 1 = Linear
    // 2 = rec709
    // 3 = Gamma 1.8
    // 4 = Gamma 2.2
    // 5 = Panalog
    // 6 = REDLog
    // 7 = ViperLog
    // 8 = AlexaV3LogC
    // 9 = PLogLin
    // 10 = SLog
    // 11 = Raw
    // If space is Linear, no conversion necessary

     const char* space;

     switch(colorSpace)
     {
         case 0:
             space = "sRGB";
             break;
         case 2:
             space = "rec709";
             break;
         case 3:
             space = "Gamma1.8";
             break;
         case 4:
             space = "Gamma2.2";
             break;
         case 5:
             space = "Panalog";
             break;
         case 6:
             space = "REDLog";
             break;
         case 7:
             space = "ViperLog";
             break;
         case 8:
             space = "AlexaV3LogC";
             break;
         case 9:
             space = "PLogLin";
             break;
         case 10:
             space = "SLog";
             break;
         case 11:
             space = "raw";
             break;
         default:
             space = "Linear";
      }

    auto processor = ocioConfig->getProcessor("Linear", space);
    OCIO::PackedImageDesc img(
                static_cast<float*>(cpuImage->localpixels()),
                cpuImage->xmax(),
                cpuImage->ymax(),
                4);
    processor->apply(img);

    updateVertexData(cpuImage->xend(), cpuImage->yend());

    imageFromDisk = std::unique_ptr<CsImage>(new CsImage(
                                                 window,
                                                 &device,
                                                 devFuncs,
                                                 cpuImage->xend(),
                                                 cpuImage->yend()));

    auto imageSize = QSize(cpuImage->xend(), cpuImage->yend());

    // Now we can either map and copy the image data directly, or have to go
    // through a staging buffer to copy and convert into the internal optimal
    // tiling format.
    VkFormatProperties props;
    f->vkGetPhysicalDeviceFormatProperties(window->physicalDevice(), globalImageFormat, &props);
    const bool canSampleLinear = (props.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    const bool canSampleOptimal = (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    if (!canSampleLinear && !canSampleOptimal) {
        qWarning("Neither linear nor optimal image sampling is supported for image");
        return false;
    }

    if (loadImageStaging) {
        devFuncs->vkDestroyImage(device, loadImageStaging, nullptr);
        loadImageStaging = VK_NULL_HANDLE;
    }

    if (loadImageStagingMem) {
        devFuncs->vkFreeMemory(device, loadImageStagingMem, nullptr);
        loadImageStagingMem = VK_NULL_HANDLE;
    }

    if (!createTextureImage(imageSize, &loadImageStaging, &loadImageStagingMem,
                            VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                            window->hostVisibleMemoryIndex()))
        return false;

    if (!writeLinearImage(*cpuImage, loadImageStaging, loadImageStagingMem))
        return false;

    texStagingPending = true;

    VkImageViewCreateInfo viewInfo;
    memset(&viewInfo, 0, sizeof(viewInfo));
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = imageFromDisk->getImage();
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = globalImageFormat;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = viewInfo.subresourceRange.layerCount = 1;

    VkResult err = devFuncs->vkCreateImageView(device, &viewInfo, nullptr, &imageFromDisk->getImageView());
    if (err != VK_SUCCESS) {
        qWarning("Failed to create image view for texture: %d", err);
        return false;
    }

    loadImageSize = imageSize;

    return true;
}

void VulkanRenderer::createComputeDescriptors()
{
    if (computeDescriptorSetLayoutOneInput == VK_NULL_HANDLE)
    {
        // Define the layout of the input of the shader.
        // 1 image to read, 1 image to write
        VkDescriptorSetLayoutBinding bindings[2]= {};

        bindings[0].binding         = 0;
        bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

        bindings[1].binding         = 1;
        bindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.pBindings = bindings;
        descriptorSetLayoutCreateInfo.bindingCount = 2;

        //Create the layout, store it to share between shaders
        VkResult err = devFuncs->vkCreateDescriptorSetLayout(
                    device,
                    &descriptorSetLayoutCreateInfo,
                    nullptr,
                    &computeDescriptorSetLayoutOneInput);
        if (err != VK_SUCCESS)
            qFatal("Failed to create compute descriptor set layout: %d", err);
    }

    if (computeDescriptorSetLayoutTwoInputs == VK_NULL_HANDLE)
    {
        // Define the layout of the input of the shader.
        // 2 images to read, 1 image to write
        VkDescriptorSetLayoutBinding bindings[3]= {};

        bindings[0].binding         = 0;
        bindings[0].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

        bindings[1].binding         = 1;
        bindings[1].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

        bindings[2].binding         = 2;
        bindings[2].descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        bindings[2].descriptorCount = 1;
        bindings[2].stageFlags      = VK_SHADER_STAGE_COMPUTE_BIT;

        VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo {};
        descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        descriptorSetLayoutCreateInfo.pBindings = bindings;
        descriptorSetLayoutCreateInfo.bindingCount = 3;

        //Create the layout, store it to share between shaders
        VkResult err = devFuncs->vkCreateDescriptorSetLayout(
                    device,
                    &descriptorSetLayoutCreateInfo,
                    nullptr,
                    &computeDescriptorSetLayoutTwoInputs);
        if (err != VK_SUCCESS)
            qFatal("Failed to create compute descriptor set layout: %d", err);
    }

    // Descriptor sets
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        {
            VkDescriptorSetAllocateInfo descSetAllocInfo = {
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                nullptr,
                descriptorPool,
                1,
                &graphicsDescriptorSetLayout
            };
            VkResult err = devFuncs->vkAllocateDescriptorSets(
                        device,
                        &descSetAllocInfo,
                        &graphicsDescriptorSet[i]);
            if (err != VK_SUCCESS)
                qFatal("Failed to allocate descriptor set: %d", err);
        }

        {
            VkDescriptorSetAllocateInfo descSetAllocInfo = {
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                nullptr,
                descriptorPool,
                1,
                &computeDescriptorSetLayoutOneInput
            };
            VkResult err = devFuncs->vkAllocateDescriptorSets(
                        device,
                        &descSetAllocInfo,
                        &computeDescriptorSetOneInput);
            if (err != VK_SUCCESS)
                qFatal("Failed to allocate descriptor set: %d", err);
        }

        {
            VkDescriptorSetAllocateInfo descSetAllocInfo = {
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                nullptr,
                descriptorPool,
                1,
                &computeDescriptorSetLayoutTwoInputs
            };
            VkResult err = devFuncs->vkAllocateDescriptorSets(
                        device,
                        &descSetAllocInfo,
                        &computeDescriptorSetTwoInputs);
            if (err != VK_SUCCESS)
                qFatal("Failed to allocate descriptor set: %d", err);
        }
    }
}

void VulkanRenderer::updateComputeDescriptors(
        CsImage& inputImageBack,
        CsImage& outputImage)
{
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        VkWriteDescriptorSet descWrite[2];
        memset(descWrite, 0, sizeof(descWrite));
        descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet = graphicsDescriptorSet[i];
        descWrite[0].dstBinding = 0;
        descWrite[0].descriptorCount = 1;
        descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descWrite[0].pBufferInfo = &uniformBufInfo[i];

        VkDescriptorImageInfo descImageInfo = {
            sampler,
            outputImage.getImageView(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet = graphicsDescriptorSet[i];
        descWrite[1].dstBinding = 1;
        descWrite[1].descriptorCount = 1;
        descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descWrite[1].pImageInfo = &descImageInfo;

        devFuncs->vkUpdateDescriptorSets(device, 2, descWrite, 0, nullptr);
    }

    {
        VkDescriptorImageInfo destinationInfo = { };
        destinationInfo.imageView             = outputImage.getImageView();
        destinationInfo.imageLayout           = VK_IMAGE_LAYOUT_GENERAL;

        VkDescriptorImageInfo sourceInfo      = { };
        sourceInfo.imageView                  = inputImageBack.getImageView();
        sourceInfo.imageLayout                = VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet descWrite[2]= {};

        descWrite[0].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet                    = computeDescriptorSetOneInput;
        descWrite[0].dstBinding                = 0;
        descWrite[0].descriptorCount           = 1;
        descWrite[0].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[0].pImageInfo                = &sourceInfo;

        descWrite[1].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet                    = computeDescriptorSetOneInput;
        descWrite[1].dstBinding                = 1;
        descWrite[1].descriptorCount           = 1;
        descWrite[1].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[1].pImageInfo                = &destinationInfo;
        devFuncs->vkUpdateDescriptorSets(device, 2, descWrite, 0, nullptr);
    }
}

// TODO: This should be merged with function above
void VulkanRenderer::updateComputeDescriptors(
        CsImage& inputImageBack,
        CsImage& inputImageFront,
        CsImage& outputImage)
{
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        VkWriteDescriptorSet descWrite[2];
        memset(descWrite, 0, sizeof(descWrite));
        descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet = graphicsDescriptorSet[i];
        descWrite[0].dstBinding = 0;
        descWrite[0].descriptorCount = 1;
        descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descWrite[0].pBufferInfo = &uniformBufInfo[i];

        VkDescriptorImageInfo descImageInfo = {
            sampler,
            outputImage.getImageView(),
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet = graphicsDescriptorSet[i];
        descWrite[1].dstBinding = 1;
        descWrite[1].descriptorCount = 1;
        descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descWrite[1].pImageInfo = &descImageInfo;

        devFuncs->vkUpdateDescriptorSets(device, 2, descWrite, 0, nullptr);
    }

    {
        VkDescriptorImageInfo sourceInfoBack     = { };
        sourceInfoBack.imageView                 = inputImageBack.getImageView();
        sourceInfoBack.imageLayout               = VK_IMAGE_LAYOUT_GENERAL;

        VkDescriptorImageInfo sourceInfoFront     = { };
        sourceInfoFront.imageView                 = inputImageFront.getImageView();
        sourceInfoFront.imageLayout               = VK_IMAGE_LAYOUT_GENERAL;

        VkDescriptorImageInfo destinationInfo     = { };
        destinationInfo.imageView                 = outputImage.getImageView();
        destinationInfo.imageLayout               = VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet descWrite[3]= {};

        descWrite[0].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet                    = computeDescriptorSetTwoInputs;
        descWrite[0].dstBinding                = 0;
        descWrite[0].descriptorCount           = 1;
        descWrite[0].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[0].pImageInfo                = &sourceInfoBack;

        descWrite[1].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet                    = computeDescriptorSetTwoInputs;
        descWrite[1].dstBinding                = 1;
        descWrite[1].descriptorCount           = 1;
        descWrite[1].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[1].pImageInfo                = &sourceInfoFront;

        descWrite[2].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[2].dstSet                    = computeDescriptorSetTwoInputs;
        descWrite[2].dstBinding                = 2;
        descWrite[2].descriptorCount           = 1;
        descWrite[2].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[2].pImageInfo                = &destinationInfo;
        devFuncs->vkUpdateDescriptorSets(device, 3, descWrite, 0, nullptr);
    }
}

void VulkanRenderer::createComputePipelineLayout()
{
    VkPushConstantRange pushConstantRange;
    pushConstantRange.stageFlags                    = VK_SHADER_STAGE_COMPUTE_BIT;
    // Compute constants come after fragment constants
    pushConstantRange.offset                        = 0;
    pushConstantRange.size                          = sizeof(computePushConstants) * 16;

    {
        //Now create the layout info
        VkPipelineLayoutCreateInfo pipelineLayoutInfo   = {};
        pipelineLayoutInfo.sType                        = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount               = 1;
        pipelineLayoutInfo.pSetLayouts                  = &computeDescriptorSetLayoutOneInput;
        pipelineLayoutInfo.pushConstantRangeCount       = 1;
        pipelineLayoutInfo.pPushConstantRanges          = &pushConstantRange;

        //Create the layout, store it to share between shaders
        VkResult err = devFuncs->vkCreatePipelineLayout(
                    device,
                    &pipelineLayoutInfo,
                    nullptr,
                    &computePipelineLayoutOneInput);
        if (err != VK_SUCCESS)
            qFatal("Failed to create compute pipeline layout: %d", err);
    }

    {
        //Now create the layout info
        VkPipelineLayoutCreateInfo pipelineLayoutInfo   = {};
        pipelineLayoutInfo.sType                        = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount               = 1;
        pipelineLayoutInfo.pSetLayouts                  = &computeDescriptorSetLayoutTwoInputs;
        pipelineLayoutInfo.pushConstantRangeCount       = 1;
        pipelineLayoutInfo.pPushConstantRanges          = &pushConstantRange;

        //Create the layout, store it to share between shaders
        VkResult err = devFuncs->vkCreatePipelineLayout(
                    device,
                    &pipelineLayoutInfo,
                    nullptr,
                    &computePipelineLayoutTwoInputs);
        if (err != VK_SUCCESS)
            qFatal("Failed to create compute pipeline layout: %d", err);
    }

}


void VulkanRenderer::createComputePipelines()
{
    for (int i = 0; i != NODE_TYPE_MAX; i++)
    {
        NodeType nodeType = static_cast<NodeType>(i);

        pipelines[nodeType] = createComputePipeline(nodeType);
    }
}

VkPipeline VulkanRenderer::createComputePipeline(NodeType nodeType)
{
    auto shaderModule = shaders[nodeType];

    VkPipelineShaderStageCreateInfo computeStage = {

            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            VK_SHADER_STAGE_COMPUTE_BIT,
            shaderModule,
            "main",
            nullptr
        };

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage  = computeStage;
    if (getPropertiesForType(nodeType).nodeInputs.size() > 1)
    {
        pipelineInfo.layout = computePipelineLayoutTwoInputs;
    }
    else
    {
        pipelineInfo.layout = computePipelineLayoutOneInput;
    }

    VkPipeline pl = VK_NULL_HANDLE;

    VkResult err = devFuncs->vkCreateComputePipelines(
                device,
                pipelineCache,
                1,
                &pipelineInfo,
                nullptr,
                &pl);
    if (err != VK_SUCCESS)
        qFatal("Failed to create compute pipeline: %d", err);

    return pl;
}

VkPipeline VulkanRenderer::createComputePipelineNoop()
{
    auto shaderModule = createShaderFromFile(":/shaders/noop_comp.spv");;

    VkPipelineShaderStageCreateInfo computeStage = {

            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            VK_SHADER_STAGE_COMPUTE_BIT,
            shaderModule,
            "main",
            nullptr
        };

    VkComputePipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage  = computeStage;
    pipelineInfo.layout = computePipelineLayoutOneInput;

    VkPipeline pl = VK_NULL_HANDLE;

    VkResult err = devFuncs->vkCreateComputePipelines(
                device,
                pipelineCache,
                1,
                &pipelineInfo,
                nullptr,
                &pl);
    if (err != VK_SUCCESS)
        qFatal("Failed to create compute pipeline: %d", err);

    return pl;

}

void VulkanRenderer::createComputeQueue()
{
    VkPhysicalDevice physicalDevice = window->physicalDevice();

    uint32_t queueFamilyCount;
    f->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    queueFamilyProperties.resize(queueFamilyCount);

    f->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

    bool computeQueueFound = false;
    for (auto i = 0U; i < queueFamilyProperties.size(); ++i)
    {
        if ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
        {
            compute.queueFamilyIndex = i;
            computeQueueFound = true;
            break;
        }
    }

    computeQueueFound = false;

    // If there is no dedicated compute queue, just find the first queue family that supports compute
    if (!computeQueueFound)
    {
        for (auto i = 0U; i < queueFamilyProperties.size(); ++i)
        {
            if (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                compute.queueFamilyIndex = i;
                computeQueueFound = true;
                break;
            }
        }
    }

    // Get a compute queue from the device
    devFuncs->vkGetDeviceQueue(device, compute.queueFamilyIndex, 0, &compute.computeQueue);
}

void VulkanRenderer::createComputeCommandPool()
{
    // Separate command pool as queue family for compute may be different than graphics
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = compute.queueFamilyIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkResult err;

    err = devFuncs->vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &compute.computeCommandPool);

    if (err != VK_SUCCESS)
        qFatal("Failed to create compute command pool: %d", err);
}

void VulkanRenderer::createQueryPool()
{
    VkQueryPoolCreateInfo queryPooloolInfo = {};
    queryPooloolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
    queryPooloolInfo.pNext = nullptr;
    queryPooloolInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
    queryPooloolInfo.queryCount = 2;

    VkResult err = devFuncs->vkCreateQueryPool(device, &queryPooloolInfo, nullptr, &queryPool);
    if (err != VK_SUCCESS)
        qFatal("Failed to create query pool: %d", err);
}

void VulkanRenderer::createComputeCommandBuffer()
{
    // Create the command buffer for loading an image from disk
    VkCommandBufferAllocateInfo commandBufferAllocateInfo {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = compute.computeCommandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 4;

    VkCommandBuffer buffers[4] = {};
    VkResult err = devFuncs->vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &buffers[0]);

    if (err != VK_SUCCESS)
        qFatal("Failed to allocate descriptor set: %d", err);

    compute.commandBufferOneInput  = buffers[0];
    compute.commandBufferImageLoad = buffers[1];
    compute.commandBufferTwoInputs = buffers[2];
    compute.commandBufferImageSave = buffers[3];

    // Fence for compute CB sync
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    err = devFuncs->vkCreateFence(device, &fenceCreateInfo, nullptr, &compute.fence);

    if (err != VK_SUCCESS)
        qFatal("Failed to create fence: %d", err);

    // Flush the queue if we're rebuilding the command buffer
    // after a pipeline change to ensure it's not currently in use
    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    err = devFuncs->vkBeginCommandBuffer(compute.commandBufferImageLoad, &cmdBufferBeginInfo);

    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);

    VkCommandBuffer cb = compute.commandBufferImageLoad;

    // Create barriers for the resources
    VkImageMemoryBarrier barrier = {};
    memset(&barrier, 0, sizeof(barrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = barrier.subresourceRange.layerCount = 1;

    barrier.oldLayout       = VK_IMAGE_LAYOUT_PREINITIALIZED;
    barrier.newLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask   = VK_ACCESS_HOST_WRITE_BIT;
    barrier.dstAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.image           = loadImageStaging;

    devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_HOST_BIT,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            1, &barrier);

    barrier.oldLayout       = VK_IMAGE_LAYOUT_PREINITIALIZED;
    barrier.newLayout       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcAccessMask   = 0;
    barrier.dstAccessMask   = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.image           = imageFromDisk->getImage();

    devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            0,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            1, &barrier);

    VkImageCopy copyInfo;
    memset(&copyInfo, 0, sizeof(copyInfo));
    copyInfo.srcSubresource.aspectMask  = VK_IMAGE_ASPECT_COLOR_BIT;
    copyInfo.srcSubresource.layerCount  = 1;
    copyInfo.dstSubresource.aspectMask  = VK_IMAGE_ASPECT_COLOR_BIT;
    copyInfo.dstSubresource.layerCount  = 1;
    copyInfo.extent.width               = loadImageSize.width();
    copyInfo.extent.height              = loadImageSize.height();
    copyInfo.extent.depth               = 1;

    devFuncs->vkCmdCopyImage(
                cb,
                loadImageStaging,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                imageFromDisk->getImage(),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &copyInfo);

    {
        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.image         = imageFromDisk->getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);

        barrier.oldLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout     = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.image         = computeRenderTarget->getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0,
                                nullptr,
                                0,
                                nullptr,
                                1,
                                &barrier);
    }

    devFuncs->vkCmdBindPipeline(
                compute.commandBufferImageLoad,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                pipelines[NODE_TYPE_READ]);
    devFuncs->vkCmdBindDescriptorSets(
                compute.commandBufferImageLoad,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                computePipelineLayoutOneInput,
                0,
                1,
                &computeDescriptorSetOneInput,
                0,
                0);
    devFuncs->vkCmdDispatch(
                compute.commandBufferImageLoad,
                cpuImage->xend() / 16 + 1,
                cpuImage->yend() / 16 + 1, 1);

    {
       //Make the barriers for the resources
       VkImageMemoryBarrier barrier[2] = {};

        barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

        barrier[0].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[0].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[0].srcAccessMask   = 0;
        barrier[0].dstAccessMask   = 0;
        barrier[0].image           = imageFromDisk->getImage();

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = computeRenderTarget->getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            2, &barrier[0]);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBufferImageLoad);
}


bool VulkanRenderer::createTextureImage(
                const QSize &size,
                VkImage *image,
                VkDeviceMemory *mem,
                VkImageTiling tiling,
                VkImageUsageFlags usage,
                uint32_t memIndex)
{
    VkImageCreateInfo imageInfo;
    memset(&imageInfo, 0, sizeof(imageInfo));
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = globalImageFormat;
    imageInfo.extent.width = size.width();
    imageInfo.extent.height = size.height();
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;

    VkResult err = devFuncs->vkCreateImage(device, &imageInfo, nullptr, image);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create linear image for texture: %d", err);
        return false;
    }

    VkMemoryRequirements memReq;
    devFuncs->vkGetImageMemoryRequirements(device, *image, &memReq);

    if (!(memReq.memoryTypeBits & (1 << memIndex))) {
        VkPhysicalDeviceMemoryProperties physDevMemProps;
        window->vulkanInstance()->functions()->vkGetPhysicalDeviceMemoryProperties(
                    window->physicalDevice(), &physDevMemProps);
        for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i) {
            if (!(memReq.memoryTypeBits & (1 << i)))
                continue;
            memIndex = i;
        }
    }

    VkMemoryAllocateInfo allocInfo = {
        VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        nullptr,
        memReq.size,
        memIndex
    };
    qDebug("allocating %u bytes for texture image", uint32_t(memReq.size));

    err = devFuncs->vkAllocateMemory(device, &allocInfo, nullptr, mem);
    if (err != VK_SUCCESS) {
        qWarning("Failed to allocate memory for linear image: %d", err);
        return false;
    }

    err = devFuncs->vkBindImageMemory(device, *image, *mem, 0);
    if (err != VK_SUCCESS) {
        qWarning("Failed to bind linear image memory: %d", err);
        return false;
    }

    return true;
}

bool VulkanRenderer::writeLinearImage(const ImageBuf &img, VkImage image, VkDeviceMemory memory)
{
    VkImageSubresource subres = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0, // mip level
        0
    };
    VkSubresourceLayout layout;
    devFuncs->vkGetImageSubresourceLayout(device, image, &subres, &layout);

    float *p;
    VkResult err = devFuncs->vkMapMemory(
                device,
                memory,
                layout.offset,
                layout.size,
                0,
                reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS) {
        qWarning("Failed to map memory for linear image: %d", err);
        return false;
    }

    float* pixels = (float*)img.localpixels();
    int lineWidth = img.xend() * 16; // 4 channels * 4 bytes
    for (int y = 0; y < img.yend(); ++y)
    {
        memcpy(p, pixels, lineWidth);
        pixels += img.xend() * 4;
        p += img.xend() * 4;
    }

    devFuncs->vkUnmapMemory(device, memory);

    return true;
}

void VulkanRenderer::updateVertexData( int w, int h)
{
    vertexData[0]  = -0.002 * w;
    vertexData[5]  = -0.002 * w;
    vertexData[10] = 0.002 * w;
    vertexData[15] = 0.002 * w;
    vertexData[1]  = -0.002 * h;
    vertexData[6]  = 0.002 * h;
    vertexData[11] = -0.002 * h;
    vertexData[16] = 0.002 * h;
}

void VulkanRenderer::initSwapChainResources()
{
    qDebug("initSwapChainResources");

    // Projection matrix
    projection = window->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
    const QSize sz = window->swapChainImageSize();
    projection.ortho( -sz.width() / scaleXY, sz.width() / scaleXY, -sz.height() / scaleXY, sz.height() / scaleXY, -1.0f, 100.0f);
    projection.scale(500);
}

void VulkanRenderer::recordComputeCommandBuffer(
        CsImage& inputImageBack,
        CsImage& outputImage,
        VkPipeline& pl,
        int numShaderPasses)
{
    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult err = devFuncs->vkBeginCommandBuffer(
                compute.commandBufferOneInput,
                &cmdBufferBeginInfo);
    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);
    VkCommandBuffer cb = compute.commandBufferOneInput;

    VkImage* targetImage;

    targetImage = &outputImage.getImage();

    if (numShaderPasses > 1)
    {
        computePushConstants.push_back(0.0);

        intermediateImage = std::unique_ptr<CsImage>(new CsImage(
                    window,
                    &device,
                    devFuncs,
                    outputImage.getWidth(),
                    outputImage.getHeight()));
        targetImage = &intermediateImage->getImage();

        updateComputeDescriptors(inputImageBack, *intermediateImage);
    }

    {
         //Make the barriers for the resources
        VkImageMemoryBarrier barrier[2] = {};

        barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

        barrier[0].oldLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[0].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[0].srcAccessMask   = 0;
        barrier[0].dstAccessMask   = 0;
        barrier[0].image           = inputImageBack.getImage();


        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = *targetImage;

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0,
                                nullptr,
                                0,
                                nullptr,
                                2,
                                &barrier[0]);
    }

    // Push constants for fragment stage
    devFuncs->vkCmdPushConstants(
                compute.commandBufferOneInput,
                pipelineLayout,
                VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(viewerPushConstants),
                viewerPushConstants.data());
    // Push constants for compute stage
    devFuncs->vkCmdPushConstants(
                compute.commandBufferOneInput,
                computePipelineLayoutOneInput,
                VK_SHADER_STAGE_COMPUTE_BIT,
                0,
                sizeof(computePushConstants),
                computePushConstants.data());
    devFuncs->vkCmdBindPipeline(
                compute.commandBufferOneInput,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                pl);
    devFuncs->vkCmdBindDescriptorSets(
                compute.commandBufferOneInput,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                computePipelineLayoutOneInput, 0, 1,
                &computeDescriptorSetOneInput, 0, 0);
    // Adding one extra local workgroup here to
    // prevent flickering in crop shader
    devFuncs->vkCmdDispatch(
                compute.commandBufferOneInput,
                outputImage.getWidth() / 16 + 1,
                outputImage.getHeight() / 16 + 1, 1);

    if (numShaderPasses > 1)
    {
        {
            VkImageMemoryBarrier barrier[2] = {};

            barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

            barrier[0].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
            barrier[0].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
            barrier[0].srcAccessMask   = 0;
            barrier[0].dstAccessMask   = 0;
            barrier[0].image           = intermediateImage->getImage();

            barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

            barrier[1].oldLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier[1].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
            barrier[1].srcAccessMask   = 0;
            barrier[1].dstAccessMask   = 0;
            barrier[1].image           = outputImage.getImage();

            devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0,
                                nullptr,
                                0,
                                nullptr,
                                2,
                                &barrier[0]);
        }

        computePushConstants[computePushConstants.size() - 1] += 1.0;

        updateComputeDescriptors(*intermediateImage, outputImage);

        devFuncs->vkCmdBindDescriptorSets(
                    compute.commandBufferOneInput,
                    VK_PIPELINE_BIND_POINT_COMPUTE,
                    computePipelineLayoutOneInput, 0, 1,
                    &computeDescriptorSetOneInput, 0, 0);

        devFuncs->vkCmdPushConstants(
                    compute.commandBufferOneInput,
                    computePipelineLayoutOneInput,
                    VK_SHADER_STAGE_COMPUTE_BIT,
                    0,
                    sizeof(computePushConstants),
                    computePushConstants.data());

        devFuncs->vkCmdDispatch(
                    compute.commandBufferOneInput,
                    outputImage.getWidth() / 16 + 1,
                    outputImage.getHeight() / 16 + 1, 1);
    }

    {

        VkImageMemoryBarrier barrier[2] = {};

        barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

        barrier[0].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[0].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[0].srcAccessMask   = 0;
        barrier[0].dstAccessMask   = 0;
        barrier[0].image           = inputImageBack.getImage();

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = outputImage.getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            0,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            2,
                            &barrier[0]);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBufferOneInput);
}

void VulkanRenderer::recordComputeCommandBuffer(
        CsImage& inputImageBack,
        CsImage& inputImageFront,
        CsImage& outputImage,
        VkPipeline& pl)
{
    // TODO: Merge with function above

    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult err = devFuncs->vkBeginCommandBuffer(
                compute.commandBufferTwoInputs,
                &cmdBufferBeginInfo);
    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);

     VkCommandBuffer cb = compute.commandBufferTwoInputs;

     {
         //Make the barriers for the resources
        VkImageMemoryBarrier barrier[3] = {};

        barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

        barrier[0].oldLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[0].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[0].srcAccessMask   = 0;
        barrier[0].dstAccessMask   = 0;
        barrier[0].image           = inputImageBack.getImage();

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = inputImageFront.getImage();

        barrier[2].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[2].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[2].subresourceRange.levelCount = barrier[2].subresourceRange.layerCount = 1;

        barrier[2].oldLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier[2].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[2].srcAccessMask   = 0;
        barrier[2].dstAccessMask   = 0;
        barrier[2].image           = outputImage.getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0,
                                0,
                                nullptr,
                                0,
                                nullptr,
                                3,
                                &barrier[0]);
    }

     // Push constants for fragment stage
     devFuncs->vkCmdPushConstants(
                 compute.commandBufferOneInput,
                 pipelineLayout,
                 VK_SHADER_STAGE_FRAGMENT_BIT,
                 0,
                 sizeof(viewerPushConstants),
                 viewerPushConstants.data());
     // Push constants for compute stage
    devFuncs->vkCmdPushConstants(
                compute.commandBufferTwoInputs,
                computePipelineLayoutTwoInputs,
                VK_SHADER_STAGE_COMPUTE_BIT,
                0,
                sizeof(computePushConstants),
                computePushConstants.data());
    devFuncs->vkCmdBindPipeline(
                compute.commandBufferTwoInputs,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                pl);
    devFuncs->vkCmdBindDescriptorSets(
                compute.commandBufferTwoInputs,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                computePipelineLayoutTwoInputs, 0, 1,
                &computeDescriptorSetTwoInputs, 0, 0);
    devFuncs->vkCmdDispatch(
                compute.commandBufferTwoInputs,
                outputImage.getWidth() / 16 + 1,
                outputImage.getHeight() / 16 + 1, 1);

    {
       //Make the barriers for the resources
       VkImageMemoryBarrier barrier[3] = {};

        barrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[0].subresourceRange.levelCount = barrier[0].subresourceRange.layerCount = 1;

        barrier[0].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[0].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[0].srcAccessMask   = 0;
        barrier[0].dstAccessMask   = 0;
        barrier[0].image           = inputImageBack.getImage();

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = inputImageFront.getImage();

        barrier[2].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[2].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[2].subresourceRange.levelCount = barrier[2].subresourceRange.layerCount = 1;

        barrier[2].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[2].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[2].srcAccessMask   = 0;
        barrier[2].dstAccessMask   = 0;
        barrier[2].image           = outputImage.getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            0,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            3,
                            &barrier[0]);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBufferTwoInputs);
}

uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    f->vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

void VulkanRenderer::recordComputeCommandBuffer(
        CsImage& inputImage)
{
    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult err = devFuncs->vkBeginCommandBuffer(
                compute.commandBufferImageSave,
                &cmdBufferBeginInfo);
    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);

    VkCommandBuffer cb = compute.commandBufferImageSave;

    outputImageSize = QSize(inputImage.getWidth(), inputImage.getHeight());

    VkDeviceSize bufferSize = outputImageSize.width() * outputImageSize.height() * 16; // 4 channels * 4 bytes

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (devFuncs->vkCreateBuffer(device, &bufferInfo, nullptr, &outputStagingBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    devFuncs->vkGetBufferMemoryRequirements(device, outputStagingBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
                memRequirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (devFuncs->vkAllocateMemory(device, &allocInfo, nullptr, &outputStagingBufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    devFuncs->vkBindBufferMemory(device, outputStagingBuffer, outputStagingBufferMemory, 0);

    {
        VkImageMemoryBarrier imageBarrier = {};

        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageBarrier.subresourceRange.levelCount = imageBarrier.subresourceRange.layerCount = 1;

        imageBarrier.oldLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageBarrier.newLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        imageBarrier.srcAccessMask   = 0;
        imageBarrier.dstAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
        imageBarrier.image           = inputImage.getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                0,
                                0,
                                nullptr,
                                0,
                                nullptr,
                                1,
                                &imageBarrier);
    }

    VkImageSubresource subres = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        1
    };

    VkSubresourceLayout imageLayout;
    devFuncs->vkGetImageSubresourceLayout(device, inputImage.getImage(), &subres, &imageLayout);

    VkImageSubresourceLayers imageLayers =
    {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0,
        0,
        1
    };

    VkBufferImageCopy copyInfo;
    copyInfo.bufferOffset       = 0;
    copyInfo.bufferRowLength    = outputImageSize.width();
    copyInfo.bufferImageHeight  = outputImageSize.height();
    copyInfo.imageSubresource   = imageLayers;
    copyInfo.imageOffset        = { 0, 0, 0 };
    copyInfo.imageExtent.width  = outputImageSize.width();
    copyInfo.imageExtent.height = outputImageSize.height();
    copyInfo.imageExtent.depth  = 1;

    devFuncs->vkCmdCopyImageToBuffer(
                cb,
                inputImage.getImage(),
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                outputStagingBuffer,
                1,
                &copyInfo);

    {
       VkImageMemoryBarrier barrier = {};

        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.levelCount = barrier.subresourceRange.layerCount = 1;

        barrier.oldLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask   = 0;
        barrier.image           = inputImage.getImage();

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            0,
                            0,
                            nullptr,
                            0,
                            nullptr,
                            1,
                            &barrier);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBufferImageSave);
}

void VulkanRenderer::setDisplayMode(DisplayMode mode)
{
    displayMode = mode;
}

bool VulkanRenderer::saveImageToDisk(CsImage& inputImage, const QString &path)
{
    bool success = true;

    recordComputeCommandBuffer(inputImage);
    submitImageSaveCommand();

    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    float *pInput;
    VkResult err = devFuncs->vkMapMemory(
                device,
                outputStagingBufferMemory,
                0,
                VK_WHOLE_SIZE,
                0,
                reinterpret_cast<void **>(&pInput));
    if (err != VK_SUCCESS)
    {
        qWarning("Failed to map memory for staging buffer: %d", err);
        success = false;
    }

    int width = outputImageSize.width();
    int height = outputImageSize.height();
    int numValues = width * height * 4;

    float* output = new float[numValues];
    float* pOutput = &output[0];

    for (int y = 0; y < numValues; ++y)
    {
        *pOutput = *pInput;
        pInput++;
        pOutput++;
    }

    ImageSpec spec(width, height, 4, TypeDesc::FLOAT);
    std::unique_ptr<ImageBuf> saveImage =
            std::unique_ptr<ImageBuf>(new ImageBuf(spec, output));

    // Convert linear to sRGB
    *saveImage = ImageBufAlgo::colorconvert (*saveImage, "linear", "sRGB", true);

    success = saveImage->write(path.toStdString());

    delete[] output;

    devFuncs->vkUnmapMemory(device, outputStagingBufferMemory);

    return success;
}

void VulkanRenderer::createRenderPass()
{
    qDebug("Create Render Pass.");

    VkCommandBuffer cb = window->currentCommandBuffer();

    const QSize sz = window->swapChainImageSize();

    std::cout << "Swapchain image width: " << sz.width() << std::endl;
    std::cout << "Swapchain image height: " << sz.height() << std::endl;

    // Clear background
    VkClearColorValue clearColor = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
    VkClearDepthStencilValue clearDS = { 1, 0 };
    VkClearValue clearValues[2];
    memset(clearValues, 0, sizeof(clearValues));
    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    qDebug("Beginning render pass.");

    VkRenderPassBeginInfo rpBeginInfo;
    memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
    rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBeginInfo.renderPass = window->defaultRenderPass();
    rpBeginInfo.framebuffer = window->currentFramebuffer();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;
    VkCommandBuffer cmdBuf = window->currentCommandBuffer();
    devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    quint8 *p;
    VkResult err = devFuncs->vkMapMemory(
                device,
                bufMem,
                uniformBufInfo[window->currentFrame()].offset,
                UNIFORM_DATA_SIZE,
                0,
                reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS)
        qFatal("Failed to map memory: %d", err);

    QMatrix4x4 m = projection;

    QMatrix4x4 rotation;
    rotation.setToIdentity();

    QMatrix4x4 translation;
    translation.setToIdentity();
    translation.translate(position_x, position_y, position_z);

    QMatrix4x4 scale;
    scale.setToIdentity();
    scale.scale(scaleXY, scaleXY, scaleXY);

    m = m * translation * scale;

    memcpy(p, m.constData(), 16 * sizeof(float));
    devFuncs->vkUnmapMemory(device, bufMem);

    // Choose to either display RGB or Alpha
    VkPipeline pl;
    if (displayMode == DISPLAY_MODE_ALPHA)
        pl = graphicsPipelineAlpha;
    else
        pl = graphicsPipelineRGB;

    devFuncs->vkCmdBindPipeline(
                cb,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pl);
    devFuncs->vkCmdBindDescriptorSets(
                cb,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                pipelineLayout,
                0,
                1,
                &graphicsDescriptorSet[window->currentFrame()],
                0,
                nullptr);
    VkDeviceSize vbOffset = 0;
    devFuncs->vkCmdBindVertexBuffers(cb, 0, 1, &buf, &vbOffset);

    //negative viewport
    VkViewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = sz.width();
    viewport.height = sz.height();
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    devFuncs->vkCmdSetViewport(cb, 0, 1, &viewport);

    VkRect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    devFuncs->vkCmdSetScissor(cb, 0, 1, &scissor);

    devFuncs->vkCmdDraw(cb, 4, 1, 0, 0);

    devFuncs->vkCmdEndRenderPass(cmdBuf);
}

void VulkanRenderer::submitComputeCommands()
{
    // Submit compute commands
    // Use a fence to ensure that compute command buffer has finished executing before using it again
    devFuncs->vkWaitForFences(device, 1, &compute.fence, VK_TRUE, UINT64_MAX);
    devFuncs->vkResetFences(device, 1, &compute.fence);

    //Do the copy on the compute queue
    if (texStagingPending)
    {
        texStagingPending = false;
        VkSubmitInfo computeSubmitInfo {};
        computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        computeSubmitInfo.commandBufferCount = 1;
        computeSubmitInfo.pCommandBuffers = &compute.commandBufferImageLoad;
        devFuncs->vkQueueSubmit(compute.computeQueue, 1, &computeSubmitInfo, compute.fence);
    }
    else
    {
        VkSubmitInfo computeSubmitInfo {};
        computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        computeSubmitInfo.commandBufferCount = 1;
        if (!renderTwoInputs)
        {
            computeSubmitInfo.pCommandBuffers = &compute.commandBufferOneInput;
        }
        else
        {
            computeSubmitInfo.pCommandBuffers = &compute.commandBufferTwoInputs;
        }
        devFuncs->vkQueueSubmit(compute.computeQueue, 1, &computeSubmitInfo, compute.fence);
    }
}

void VulkanRenderer::submitImageSaveCommand()
{
    // Use a fence to ensure that compute command buffer has finished executing before using it again
    devFuncs->vkWaitForFences(device, 1, &compute.fence, VK_TRUE, UINT64_MAX);
    devFuncs->vkResetFences(device, 1, &compute.fence);

    VkSubmitInfo computeSubmitInfo {};
    computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    computeSubmitInfo.commandBufferCount = 1;

    computeSubmitInfo.pCommandBuffers = &compute.commandBufferImageSave;

    devFuncs->vkQueueSubmit(compute.computeQueue, 1, &computeSubmitInfo, compute.fence);
}

std::vector<float> VulkanRenderer::unpackPushConstants(const QString s)
{
    std::vector<float> values;
    auto parts = s.split(",");
    foreach(const QString& part, parts)
    {
        values.push_back(part.toFloat());
    }
    return values;
}

void VulkanRenderer::setViewerPushConstants(const QString &s)
{
    std::cout << "Setting fragment stage push constants" << std::endl;
    viewerPushConstants = unpackPushConstants(s);
}

void VulkanRenderer::processReadNode(NodeBase *node)
{
    auto parts = node->getAllPropertyValues().split(",");
    QString path = parts[0];
    int colorSpace = parts[1].toInt();

    if(path != "")
    {
        std::cout << "Processing read node." << std::endl;

        imagePath = path;

        // Create texture
        if (!createTextureFromFile(imagePath, colorSpace))
            qFatal("Failed to create texture");

        // Update the projection size
        createVertexBuffer();

        // Create render target
        if (!createComputeRenderTarget(cpuImage->xend(), cpuImage->yend()))
            qFatal("Failed to create compute render target.");

        updateComputeDescriptors(*imageFromDisk, *computeRenderTarget);

        createComputeCommandBuffer();
        recordComputeCommandBuffer(
                    *imageFromDisk,
                    *computeRenderTarget,
                    pipelines[NODE_TYPE_READ]);

        submitComputeCommands();

        node->cachedImage = std::move(computeRenderTarget);
    }
}

void VulkanRenderer::processNode(
        NodeBase* node,
        CsImage &inputImageBack,
        const QSize targetSize)
{
    renderTwoInputs = false;

    computePushConstants = unpackPushConstants(node->getAllPropertyValues());

    if (currentRenderSize != targetSize)
    {
        updateVertexData(targetSize.width(), targetSize.height());
        createVertexBuffer();
    }

    if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
        qFatal("Failed to create compute render target.");

    updateComputeDescriptors(inputImageBack, *computeRenderTarget);

    int shaderPasses = getPropertiesForType(node->nodeType).numShaderPasses;

    recordComputeCommandBuffer(
                inputImageBack,
                *computeRenderTarget,
                pipelines[node->nodeType],
                shaderPasses);

    submitComputeCommands();

    node->cachedImage = std::move(computeRenderTarget);
}

void VulkanRenderer::processNode(
        NodeBase* node,
        CsImage &inputImageBack,
        CsImage &inputImageFront,
        const QSize targetSize)
{
    // TODO: Merge with function above

    renderTwoInputs = true;

    computePushConstants = unpackPushConstants(node->getAllPropertyValues());

    if (currentRenderSize != targetSize)
    {
        updateVertexData(targetSize.width(), targetSize.height());
        createVertexBuffer();
    }

    if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
        qFatal("Failed to create compute render target.");

    updateComputeDescriptors(inputImageBack, inputImageFront, *computeRenderTarget);

    recordComputeCommandBuffer(inputImageBack, inputImageFront, *computeRenderTarget, pipelines[node->nodeType]);

    submitComputeCommands();

    node->cachedImage = std::move(computeRenderTarget);
}

void VulkanRenderer::displayNode(NodeBase *node)
{
    // Should probably use something like cmdBlitImage
    // instead of the hacky noop shader workaround
    // for displaying a node that has already been rendered

    renderTwoInputs = false; // TODO: This is a problem

    if(auto image = node->cachedImage)
    {
        clearScreen = false;

        updateVertexData(image->getWidth(), image->getHeight());
        createVertexBuffer();

        if (!createComputeRenderTarget(image->getWidth(), image->getHeight()))
            qFatal("Failed to create compute render target.");

        updateComputeDescriptors(*image, *computeRenderTarget);

        recordComputeCommandBuffer(*image, *computeRenderTarget, noopPipeline);

        submitComputeCommands();

        window->requestUpdate();
    }
    else
    {
        doClearScreen();
    }
}

void VulkanRenderer::doClearScreen()
{
    clearScreen = true;

    window->requestUpdate();
}

std::vector<char> uintVecToCharVec(const std::vector<unsigned int>& in)
{
    std::vector<char> out;

    for (size_t i = 0; i < in.size(); i++)
    {
        out.push_back(in[i] >> 0);
        out.push_back(in[i] >> 8);
        out.push_back(in[i] >> 16);
        out.push_back(in[i] >> 24);
    }

    return out;
}

void VulkanRenderer::startNextFrame()
{
    qDebug("startNextFrame");

    if (clearScreen)
    {
        const QSize sz = window->swapChainImageSize();

        // Clear background
        VkClearColorValue clearColor = {{ 0.0f, 0.0f, 0.0f, 0.0f }};
        VkClearDepthStencilValue clearDS = { 1, 0 };
        VkClearValue clearValues[2];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = clearDS;

        VkRenderPassBeginInfo rpBeginInfo;
        memset(&rpBeginInfo, 0, sizeof(rpBeginInfo));
        rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rpBeginInfo.renderPass = window->defaultRenderPass();
        rpBeginInfo.framebuffer = window->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount = 2;
        rpBeginInfo.pClearValues = clearValues;
        VkCommandBuffer cmdBuf = window->currentCommandBuffer();
        devFuncs->vkCmdBeginRenderPass(cmdBuf, &rpBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        devFuncs->vkCmdEndRenderPass(cmdBuf);
    }
    else
    {
        createRenderPass();
    }

    window->frameReady();
}

void VulkanRenderer::logicalDeviceLost()
{
    emit window->deviceLost();
}

void VulkanRenderer::translate(float dx, float dy)
{
    const QSize sz = window->size();

    position_x += 6.0 * dx / sz.width();
    position_y += 2.0 * -dy / sz.height();

    window->requestUpdate();
}

void VulkanRenderer::scale(float s)
{
    scaleXY = s;
    window->requestUpdate();
    window->requestZoomTextUpdate(s);
}

void VulkanRenderer::releaseSwapChainResources()
{
    qDebug("Releasing swapchain resources.");
}

void VulkanRenderer::releaseResources()
{
    qDebug("Releasing resources.");

    devFuncs->vkQueueWaitIdle(compute.computeQueue);

    if (queryPool) {
        devFuncs->vkDestroyQueryPool(device, queryPool, nullptr);
        queryPool = VK_NULL_HANDLE;
    }

    if (sampler) {
        devFuncs->vkDestroySampler(device, sampler, nullptr);
        sampler = VK_NULL_HANDLE;
    }

    if (loadImageStaging) {
        devFuncs->vkDestroyImage(device, loadImageStaging, nullptr);
        loadImageStaging = VK_NULL_HANDLE;
    }

    if (loadImageStagingMem) {
        devFuncs->vkFreeMemory(device, loadImageStagingMem, nullptr);
        loadImageStagingMem = VK_NULL_HANDLE;
    }

    if (graphicsPipelineAlpha) {
        devFuncs->vkDestroyPipeline(device, graphicsPipelineAlpha, nullptr);
        graphicsPipelineAlpha = VK_NULL_HANDLE;
    }

    if (graphicsPipelineRGB) {
        devFuncs->vkDestroyPipeline(device, graphicsPipelineRGB, nullptr);
        graphicsPipelineRGB = VK_NULL_HANDLE;
    }

    if (pipelineLayout) {
        devFuncs->vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        pipelineLayout = VK_NULL_HANDLE;
    }

    if (pipelineCache) {
        devFuncs->vkDestroyPipelineCache(device, pipelineCache, nullptr);
        pipelineCache = VK_NULL_HANDLE;
    }

    if (graphicsDescriptorSetLayout) {
        devFuncs->vkDestroyDescriptorSetLayout(device, graphicsDescriptorSetLayout, nullptr);
        graphicsDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if (descriptorPool) {
        devFuncs->vkDestroyDescriptorPool(device, descriptorPool, nullptr);
        descriptorPool = VK_NULL_HANDLE;
    }

    if (outputStagingBuffer) {
        devFuncs->vkDestroyBuffer(device, outputStagingBuffer, nullptr);
        outputStagingBuffer = VK_NULL_HANDLE;
    }

    if (outputStagingBufferMemory) {
        devFuncs->vkFreeMemory(device, outputStagingBufferMemory, nullptr);
        outputStagingBufferMemory = VK_NULL_HANDLE;
    }

    if (buf) {
        devFuncs->vkDestroyBuffer(device, buf, nullptr);
        buf = VK_NULL_HANDLE;
    }

    if (bufMem) {
        devFuncs->vkFreeMemory(device, bufMem, nullptr);
        bufMem = VK_NULL_HANDLE;
    }

    if (computeDescriptorSetLayoutOneInput) {
        devFuncs->vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayoutOneInput, nullptr);
        computeDescriptorSetLayoutOneInput = VK_NULL_HANDLE;
    }

    if (computeDescriptorSetLayoutTwoInputs) {
        devFuncs->vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayoutTwoInputs, nullptr);
        computeDescriptorSetLayoutTwoInputs = VK_NULL_HANDLE;
    }

    if (computePipeline) {
        devFuncs->vkDestroyPipeline(device, computePipeline, nullptr);
        computePipeline = VK_NULL_HANDLE;
    }

    if (computePipelineLayoutOneInput) {
        devFuncs->vkDestroyPipelineLayout(device, computePipelineLayoutOneInput, nullptr);
        computePipelineLayoutOneInput = VK_NULL_HANDLE;
    }

    if (computePipelineLayoutTwoInputs) {
        devFuncs->vkDestroyPipelineLayout(device, computePipelineLayoutTwoInputs, nullptr);
        computePipelineLayoutTwoInputs = VK_NULL_HANDLE;
    }


    if (compute.fence) {
        devFuncs->vkDestroyFence(device, compute.fence, nullptr);
        compute.fence = VK_NULL_HANDLE;
    }

    if (compute.computeCommandPool)
    {
        VkCommandBuffer buffers[3]=
        {
            compute.commandBufferOneInput,
            compute.commandBufferImageLoad,
            compute.commandBufferTwoInputs
        };

        devFuncs->vkFreeCommandBuffers(device, compute.computeCommandPool, 3, &buffers[0]);
        devFuncs->vkDestroyCommandPool(device, compute.computeCommandPool, nullptr);
    }
}

VulkanRenderer::~VulkanRenderer()
{

}
