#include "vulkanrenderer.h"

#include <iostream>

#include <QVulkanFunctions>
#include <QCoreApplication>
#include <QFile>
#include <QVulkanFunctions>
#include <QMouseEvent>
#include <QVulkanWindowRenderer>

#include "vulkanwindow.h"

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
    qDebug("initResources");

    device = window->device();
    physicalDevice = window->physicalDevice();
    devFuncs = window->vulkanInstance()->deviceFunctions(device);
    f = window->vulkanInstance()->functions();

    // Create texture
    if (!createTexture(imagePath))
        qFatal("Failed to create texture");

    createVertexBuffer();
    createSampler();
    createGraphicsDescriptors();
    createGraphicsPipelineCache();
    createGraphicsPipelineLayout();
    createGraphicsPipeline();

    //Compute
    // Create render target
    if (!createComputeRenderTarget(cpuImage.width(), cpuImage.height()))
        qFatal("Failed to create compute render target.");

    createQueryPool();
    createComputeDescriptors();
    createComputePipelineLayout();
    createComputePipeline();
    createComputeQueue();
    createComputeCommandPool();
    createComputeCommandBuffer();

    recordComputeCommandBuffer();

    emit window->rendererHasBeenCreated();
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
    // Create vertex buffer
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
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    VkResult err = devFuncs->vkCreateSampler(device, &samplerInfo, nullptr, &sampler);
    if (err != VK_SUCCESS)
        qFatal("Failed to create sampler: %d", err);
}

void VulkanRenderer::createGraphicsDescriptors()
{
    // Create descriptor pool
    VkDescriptorPoolSize descPoolSizes[3] = {
        { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 * uint32_t(concurrentFrameCount) },
        { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 * uint32_t(concurrentFrameCount) },
        { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          2 * uint32_t(concurrentFrameCount) } //two per frame
    };
    VkDescriptorPoolCreateInfo descPoolInfo;
    memset(&descPoolInfo, 0, sizeof(descPoolInfo));
    descPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descPoolInfo.maxSets        = 4;
    descPoolInfo.poolSizeCount  = 3;
    descPoolInfo.pPoolSizes = descPoolSizes;
    VkResult err = devFuncs->vkCreateDescriptorPool(device, &descPoolInfo, nullptr, &descPool);
    if (err != VK_SUCCESS)
        qFatal("Failed to create descriptor pool: %d", err);

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
    err = devFuncs->vkCreateDescriptorSetLayout(device, &descLayoutInfo, nullptr, &descSetLayout);
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
    // Pipeline layout
    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    memset(&pipelineLayoutInfo, 0, sizeof(pipelineLayoutInfo));
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descSetLayout;
    VkResult err = devFuncs->vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    if (err != VK_SUCCESS)
        qFatal("Failed to create pipeline layout: %d", err);
}

void VulkanRenderer::createGraphicsPipeline()
{
    // Vertex and Fragment shader
    VkShaderModule vertShaderModule = createShaderFromFile(":/shaders/texture_vert.spv");
    VkShaderModule fragShaderModule = createShaderFromFile(":/shaders/texture_frag.spv");

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

    VkResult err = devFuncs->vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineInfo, nullptr, &pipeline);
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

bool VulkanRenderer::createComputeRenderTarget(uint32_t width, uint32_t height)
{
    VkImageCreateInfo imageInfo = {};

    imageInfo.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType         = VK_IMAGE_TYPE_2D;
    imageInfo.format            = VK_FORMAT_R8G8B8A8_UNORM;
    imageInfo.extent.width      = width;
    imageInfo.extent.height     = height;
    imageInfo.extent.depth      = 1;
    imageInfo.mipLevels         = 1;
    imageInfo.arrayLayers       = 1;
    imageInfo.samples           = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling            = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage             = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
    imageInfo.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;

    //Create the opaque structure that will be referenced later
    VkResult err = devFuncs->vkCreateImage(device, &imageInfo, nullptr, &computeRenderTarget);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create linear image for texture: %d", err);
        return false;
    }

    //Get how much memory do we need and how it should aligned
    VkMemoryRequirements memReq;
    devFuncs->vkGetImageMemoryRequirements(device, computeRenderTarget, &memReq);

    //The render target will be on the gpu
    uint32_t memIndex = window->deviceLocalMemoryIndex();

    if (!(memReq.memoryTypeBits & (1 << memIndex))) {
        VkPhysicalDeviceMemoryProperties physDevMemProps;
        window->vulkanInstance()->functions()->vkGetPhysicalDeviceMemoryProperties(window->physicalDevice(), &physDevMemProps);
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

    err = devFuncs->vkAllocateMemory(device, &allocInfo, nullptr, &computeRenderTargetMemory);
    if (err != VK_SUCCESS) {
        qWarning("Failed to allocate memory for linear image: %d", err);
        return false;
    }

    //Associate the image with this chunk of memory
    err = devFuncs->vkBindImageMemory(device, computeRenderTarget, computeRenderTargetMemory, 0);
    if (err != VK_SUCCESS) {
        qWarning("Failed to bind linear image memory: %d", err);
        return false;
    }

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = computeRenderTarget;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = texFormat;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = viewInfo.subresourceRange.layerCount = 1;

    err = devFuncs->vkCreateImageView(device, &viewInfo, nullptr, &computeRenderTargetView);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create image view for texture: %d", err);
        return false;
    }

    return true;
}

bool VulkanRenderer::createTexture(const QString &name)
{
    cpuImage = QImage(name);
    if (cpuImage.isNull()) {
        qWarning("Failed to load image %s", qPrintable(name));
        return false;
    }

    updateVertexData(cpuImage.width(), cpuImage.height());

    // Convert to byte ordered RGBA8. Use premultiplied alpha, see pColorBlendState in the pipeline.
    cpuImage = cpuImage.convertToFormat(QImage::Format_RGBA8888_Premultiplied);

    // Set to sRGB
    texFormat = VK_FORMAT_R8G8B8A8_SRGB;

    // Now we can either map and copy the image data directly, or have to go
    // through a staging buffer to copy and convert into the internal optimal
    // tiling format.
    VkFormatProperties props;
    f->vkGetPhysicalDeviceFormatProperties(window->physicalDevice(), texFormat, &props);
    const bool canSampleLinear = (props.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    const bool canSampleOptimal = (props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
    if (!canSampleLinear && !canSampleOptimal) {
        qWarning("Neither linear nor optimal image sampling is supported for RGBA8");
        return false;
    }

    static bool alwaysStage = true; //Force usage accross the PCI bus

    if (canSampleLinear && !alwaysStage) {
        if (!createTextureImage(cpuImage.size(),
                                &texImage, &texMem,
                                VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_SAMPLED_BIT,
                                window->hostVisibleMemoryIndex()))
            return false;

        if (!writeLinearImage(cpuImage, texImage, texMem))
            return false;

        texLayoutPending = true;
    } else {
        if (!createTextureImage(cpuImage.size(), &texStaging, &texStagingMem,
                                VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
                                window->hostVisibleMemoryIndex()))
            return false;

        if (!createTextureImage(cpuImage.size(), &texImage, &texMem,
                                VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                window->deviceLocalMemoryIndex()))
            return false;

        if (!writeLinearImage(cpuImage, texStaging, texStagingMem))
            return false;

        texStagingPending = true;
    }

    VkImageViewCreateInfo viewInfo;
    memset(&viewInfo, 0, sizeof(viewInfo));
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = texImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = texFormat;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = viewInfo.subresourceRange.layerCount = 1;

    VkResult err = devFuncs->vkCreateImageView(device, &viewInfo, nullptr, &texView);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create image view for texture: %d", err);
        return false;
    }

    texSize = cpuImage.size();

    return true;
}

void VulkanRenderer::createComputeDescriptors()
{
    if (computeDescriptorSetLayout == VK_NULL_HANDLE)
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
        VkResult err = devFuncs->vkCreateDescriptorSetLayout(device, &descriptorSetLayoutCreateInfo, nullptr, &computeDescriptorSetLayout);
        if (err != VK_SUCCESS)
            qFatal("Failed to create compute descriptor set layout: %d", err);
    }

    //Descriptor sets
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        VkDescriptorSetAllocateInfo descSetAllocInfo = {
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            nullptr,
            descPool,
            1,
            &descSetLayout
        };
        VkResult err = devFuncs->vkAllocateDescriptorSets(device, &descSetAllocInfo, &descSet[i]);
        if (err != VK_SUCCESS)
            qFatal("Failed to allocate descriptor set: %d", err);

        {
            VkDescriptorSetAllocateInfo descSetAllocInfo = {
                VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
                nullptr,
                descPool,
                1,
                &computeDescriptorSetLayout
            };
            VkResult err = devFuncs->vkAllocateDescriptorSets(device, &descSetAllocInfo, &computeDescriptorSet);
            if (err != VK_SUCCESS)
                qFatal("Failed to allocate descriptor set: %d", err);
        }
    }

    updateComputeDescriptors();
}

void VulkanRenderer::updateComputeDescriptors()
{
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        VkWriteDescriptorSet descWrite[2];
        memset(descWrite, 0, sizeof(descWrite));
        descWrite[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet = descSet[i];
        descWrite[0].dstBinding = 0;
        descWrite[0].descriptorCount = 1;
        descWrite[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descWrite[0].pBufferInfo = &uniformBufInfo[i];

        VkDescriptorImageInfo descImageInfo = {
            sampler,
            computeRenderTargetView,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        };

        descWrite[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet = descSet[i];
        descWrite[1].dstBinding = 1;
        descWrite[1].descriptorCount = 1;
        descWrite[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descWrite[1].pImageInfo = &descImageInfo;

        devFuncs->vkUpdateDescriptorSets(device, 2, descWrite, 0, nullptr);
    }

    {

        VkDescriptorImageInfo destinationInfo = { };
        destinationInfo.imageView             = computeRenderTargetView;
        destinationInfo.imageLayout           = VK_IMAGE_LAYOUT_GENERAL;

        VkDescriptorImageInfo sourceInfo      = { };
        sourceInfo.imageView                  = texView;
        sourceInfo.imageLayout                = VK_IMAGE_LAYOUT_GENERAL;

        VkWriteDescriptorSet descWrite[2]= {};

        descWrite[0].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[0].dstSet                    = computeDescriptorSet;
        descWrite[0].dstBinding                = 0;
        descWrite[0].descriptorCount           = 1;
        descWrite[0].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[0].pImageInfo                = &sourceInfo;

        descWrite[1].sType                     = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descWrite[1].dstSet                    = computeDescriptorSet;
        descWrite[1].dstBinding                = 1;
        descWrite[1].descriptorCount           = 1;
        descWrite[1].descriptorType            = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE ;
        descWrite[1].pImageInfo                = &destinationInfo;
        devFuncs->vkUpdateDescriptorSets(device, 2, descWrite, 0, nullptr);
    }
}

void VulkanRenderer::createComputePipelineLayout()
{
    //Now create the layout info
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &computeDescriptorSetLayout;

    //Create the layout, store it to share between shaders
    VkResult err = devFuncs->vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &computePipelineLayout);
    if (err != VK_SUCCESS)
        qFatal("Failed to create compute pipeline layout: %d", err);
}

void VulkanRenderer::createComputePipeline()
{
    // Loads shader and creates a pipeline
    // Shaders
    VkShaderModule computeShaderModule = createShaderFromCode(shaderCode);

    VkPipelineShaderStageCreateInfo computeStage = {

            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            nullptr,
            0,
            VK_SHADER_STAGE_COMPUTE_BIT,
            computeShaderModule,
            "main",
            nullptr
        };

    VkComputePipelineCreateInfo pipelineInfo = {};
    //pipelineInfo.pNext = nullptr;
    //pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage  = computeStage;
    pipelineInfo.layout = computePipelineLayout;

    VkResult err = devFuncs->vkCreateComputePipelines(device, pipelineCache, 1, &pipelineInfo, nullptr, &computePipeline);
    if (err != VK_SUCCESS)
        qFatal("Failed to create compute pipeline: %d", err);

    if (computeShaderModule)
        devFuncs->vkDestroyShaderModule(device, computeShaderModule, nullptr);
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
    devFuncs->vkGetDeviceQueue(device, compute.queueFamilyIndex, 0, &compute.queue);
}

void VulkanRenderer::createComputeCommandPool()
{
    // Separate command pool as queue family for compute may be different than graphics
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = compute.queueFamilyIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VkResult err;

    err = devFuncs->vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &compute.commandPool);

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
    // Create a command buffer for compute operations
    VkCommandBufferAllocateInfo commandBufferAllocateInfo {};
    commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocateInfo.commandPool = compute.commandPool;
    commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocateInfo.commandBufferCount = 2;

    VkCommandBuffer buffers[2] = {};
    VkResult err = devFuncs->vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &buffers[0]);

    if (err != VK_SUCCESS)
        qFatal("Failed to allocate descriptor set: %d", err);

    compute.commandBuffer = buffers[0];
    compute.commandBufferInit= buffers[1];

    // Fence for compute CB sync
    VkFenceCreateInfo fenceCreateInfo = {};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    err = devFuncs->vkCreateFence(device, &fenceCreateInfo, nullptr, &compute.fence);

    if (err != VK_SUCCESS)
        qFatal("Failed to create fence: %d", err);

    // Flush the queue if we're rebuilding the command buffer after a pipeline change to ensure it's not currently in use
    devFuncs->vkQueueWaitIdle(compute.queue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    err = devFuncs->vkBeginCommandBuffer(compute.commandBufferInit, &cmdBufferBeginInfo);

    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);

    VkCommandBuffer cb = compute.commandBufferInit;

    // Make the barriers for the resources
    VkImageMemoryBarrier barrier = {};
    memset(&barrier, 0, sizeof(barrier));
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.levelCount = barrier.subresourceRange.layerCount = 1;

    barrier.oldLayout       = VK_IMAGE_LAYOUT_PREINITIALIZED;
    barrier.newLayout       = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask   = VK_ACCESS_HOST_WRITE_BIT;
    barrier.dstAccessMask   = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.image           = texStaging;

    devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_HOST_BIT,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            1, &barrier);

    barrier.oldLayout       = VK_IMAGE_LAYOUT_PREINITIALIZED;
    barrier.newLayout       = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.srcAccessMask   = 0;
    barrier.dstAccessMask   = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.image           = texImage;

    devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_TRANSFER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            1, &barrier);

    VkImageCopy copyInfo;
    memset(&copyInfo, 0, sizeof(copyInfo));
    copyInfo.srcSubresource.aspectMask  = VK_IMAGE_ASPECT_COLOR_BIT;
    copyInfo.srcSubresource.layerCount  = 1;
    copyInfo.dstSubresource.aspectMask  = VK_IMAGE_ASPECT_COLOR_BIT;
    copyInfo.dstSubresource.layerCount  = 1;
    copyInfo.extent.width               = texSize.width();
    copyInfo.extent.height              = texSize.height();
    copyInfo.extent.depth               = 1;
    devFuncs->vkCmdCopyImage(cb, texStaging, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                      texImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);

    {
        barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout     = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.image         = texImage;

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TRANSFER_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);

        barrier.oldLayout     = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout     = VK_IMAGE_LAYOUT_GENERAL;
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_MEMORY_WRITE_BIT;
        barrier.image         = computeRenderTarget;

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                1, &barrier);
    }

    devFuncs->vkCmdBindPipeline(compute.commandBufferInit, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
    devFuncs->vkCmdBindDescriptorSets(compute.commandBufferInit, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout, 0, 1, &computeDescriptorSet, 0, 0);
    devFuncs->vkCmdDispatch(compute.commandBufferInit, cpuImage.width() / 16, cpuImage.height() / 16, 1);

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
        barrier[0].image           = texImage;

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = computeRenderTarget;

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            2, &barrier[0]);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBufferInit);
}


bool VulkanRenderer::createTextureImage(const QSize &size, VkImage *image, VkDeviceMemory *mem,
                                        VkImageTiling tiling, VkImageUsageFlags usage, uint32_t memIndex)
{
    VkImageCreateInfo imageInfo;
    memset(&imageInfo, 0, sizeof(imageInfo));
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = texFormat;
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
        window->vulkanInstance()->functions()->vkGetPhysicalDeviceMemoryProperties(window->physicalDevice(), &physDevMemProps);
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

bool VulkanRenderer::writeLinearImage(const QImage &img, VkImage image, VkDeviceMemory memory)
{
    VkImageSubresource subres = {
        VK_IMAGE_ASPECT_COLOR_BIT,
        0, // mip level
        0
    };
    VkSubresourceLayout layout;
    devFuncs->vkGetImageSubresourceLayout(device, image, &subres, &layout);

    uchar *p;
    VkResult err = devFuncs->vkMapMemory(device, memory, layout.offset, layout.size, 0, reinterpret_cast<void **>(&p));
    if (err != VK_SUCCESS) {
        qWarning("Failed to map memory for linear image: %d", err);
        return false;
    }

    for (int y = 0; y < img.height(); ++y) {
        const uchar *line = img.constScanLine(y);
        memcpy(p, line, img.width() * 4);
        p += layout.rowPitch;
    }

    devFuncs->vkUnmapMemory(device, memory);
    return true;
}

void VulkanRenderer::updateVertexData( int w, int h)
{
    vertexData[0] = -0.002 * w;
    vertexData[5] = -0.002 * w;
    vertexData[10] = 0.002 * w;
    vertexData[15] = 0.002 * w;
    vertexData[1] = -0.002 * h;
    vertexData[6] = 0.002 * h;
    vertexData[11] = -0.002 * h;
    vertexData[16] = 0.002 * h;
}


void VulkanRenderer::initSwapChainResources()
{
    qDebug("initSwapChainResources");

    // Projection matrix
    projection = window->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
    const QSize sz = window->swapChainImageSize();
    projection.perspective(45.0f, sz.width() / (float) sz.height(), 0.01f, 100.0f);
    projection.translate(0, 0, -4);
}

void VulkanRenderer::recordComputeCommandBuffer()
{
    // Records the compute command buffer for using the texture image
    // Needs the right render target

    // Flush the queue if we're rebuilding the command buffer after a pipeline change to ensure it's not currently in use
    devFuncs->vkQueueWaitIdle(compute.queue);

    VkCommandBufferBeginInfo cmdBufferBeginInfo {};
    cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    VkResult err = devFuncs->vkBeginCommandBuffer(compute.commandBuffer, &cmdBufferBeginInfo);
    if (err != VK_SUCCESS)
        qFatal("Failed to begin command buffer: %d", err);

     VkCommandBuffer cb = compute.commandBuffer;

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
        barrier[0].image           = texImage;

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = computeRenderTarget;

        devFuncs->vkCmdPipelineBarrier(cb,
                                VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                0, 0, nullptr, 0, nullptr,
                                2, &barrier[0]);
     }

    devFuncs->vkCmdBindPipeline(compute.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);
    devFuncs->vkCmdBindDescriptorSets(compute.commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, computePipelineLayout, 0, 1, &computeDescriptorSet, 0, 0);
    devFuncs->vkCmdDispatch(compute.commandBuffer, cpuImage.width() / 16, cpuImage.height() / 16, 1);

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
        barrier[0].image           = texImage;

        barrier[1].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier[1].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier[1].subresourceRange.levelCount = barrier[1].subresourceRange.layerCount = 1;

        barrier[1].oldLayout       = VK_IMAGE_LAYOUT_GENERAL;
        barrier[1].newLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier[1].srcAccessMask   = 0;
        barrier[1].dstAccessMask   = 0;
        barrier[1].image           = computeRenderTarget;

        devFuncs->vkCmdPipelineBarrier(cb,
                            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                            0, 0, nullptr, 0, nullptr,
                            2, &barrier[0]);
    }

    devFuncs->vkEndCommandBuffer(compute.commandBuffer);
}

void VulkanRenderer::createRenderPass()
{
    VkCommandBuffer cb = window->currentCommandBuffer();
    const QSize sz = window->swapChainImageSize();

    // Clear background
    VkClearColorValue clearColor = {{ 0.0f, 0.0f, 0.0f, 1.0f }};
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

    quint8 *p;
    VkResult err = devFuncs->vkMapMemory(device, bufMem, uniformBufInfo[window->currentFrame()].offset,
            UNIFORM_DATA_SIZE, 0, reinterpret_cast<void **>(&p));
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

    devFuncs->vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
    devFuncs->vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,
                               &descSet[window->currentFrame()], 0, nullptr);
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
        computeSubmitInfo.pCommandBuffers = &compute.commandBufferInit;
        devFuncs->vkQueueSubmit(compute.queue, 1, &computeSubmitInfo, compute.fence);
    }
    else
    {
        VkSubmitInfo computeSubmitInfo {};
        computeSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        computeSubmitInfo.commandBufferCount = 1;
        computeSubmitInfo.pCommandBuffers = &compute.commandBuffer;
        devFuncs->vkQueueSubmit(compute.queue, 1, &computeSubmitInfo, compute.fence);
    }
}

void VulkanRenderer::updateImage(const QString& path)
{
    imagePath = path;

    // Create texture
    if (!createTexture(imagePath))
        qFatal("Failed to create texture");

    // Updates the projection size
    createVertexBuffer();

    // Create render target
    if (!createComputeRenderTarget(cpuImage.width(), cpuImage.height()))
        qFatal("Failed to create compute render target.");

    updateComputeDescriptors();

    createComputeCommandBuffer();
    recordComputeCommandBuffer();

    window->requestUpdate();
}

void VulkanRenderer::updateShader(const ShaderCode& code)
{
    shaderCode = code;

    createComputePipeline();
    recordComputeCommandBuffer();

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

VkShaderModule VulkanRenderer::createShaderFromCode(const ShaderCode& code)
{
    // TODO: Refactor this!
    // If this receives empty shader code we go ahead and read the NOOP shader from disk and continue rendering.
    // Pretty bad, needs to be changed.

    if (shaderCode.size() == 0 || window->getShowOriginal())
    {
        QFile file(":/shaders/noop_comp.spv");
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning("Failed to read shader.");
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
    else
    {
        auto codeChar = uintVecToCharVec(code); // TODO: Is this necessary?

        QByteArray codeArray = QByteArray(reinterpret_cast<const char*>(codeChar.data()), codeChar.size());

        VkShaderModuleCreateInfo shaderInfo;
        memset(&shaderInfo, 0, sizeof(shaderInfo));
        shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderInfo.codeSize = codeArray.size();
        shaderInfo.pCode = reinterpret_cast<const uint32_t *>(codeArray.constData()); //reinterpret_cast<const uint32_t *>(code.data());
        VkShaderModule shaderModule;
        VkResult err = devFuncs->vkCreateShaderModule(window->device(), &shaderInfo, nullptr, &shaderModule);
        if (err != VK_SUCCESS) {
            qWarning("Failed to create shader module: %d", err);
            return VK_NULL_HANDLE;
        }

        return shaderModule;
    }
}

void VulkanRenderer::startNextFrame()
{
    qDebug("startNextFrame");

    submitComputeCommands();

    createRenderPass();

    window->frameReady();
}

void VulkanRenderer::translate(float dx, float dy)
{
    const QSize sz          = window->swapChainImageSize();
    const float speed_x     = 4.f * abs(dx) / sz.width();       // normalize according to the window size
    const float speed_y     = 4.f * abs(dy) / sz.height();
    const float sign_dx     = dx  < 0.0f ? -1.0f : 1.0f;
    const float sign_dy     = -dy < 0.0f ? -1.0f : 1.0f;        // invert y, it comes in window coordinate system

    position_x            += sign_dx * speed_x;
    position_y            += sign_dy * speed_y;

    window->requestUpdate();
}

void VulkanRenderer::scale(float s)
{
    scaleXY = s;
    window->requestUpdate();
}

void VulkanRenderer::releaseSwapChainResources()
{
    qDebug("releaseSwapChainResources");
}

void VulkanRenderer::releaseResources()
{
    qDebug("releaseResources");

    devFuncs->vkQueueWaitIdle(compute.queue);

    if (queryPool) {
        devFuncs->vkDestroyQueryPool(device, queryPool, nullptr);
        queryPool = VK_NULL_HANDLE;
    }

    if (sampler) {
        devFuncs->vkDestroySampler(device, sampler, nullptr);
        sampler = VK_NULL_HANDLE;
    }

    if (texStaging) {
        devFuncs->vkDestroyImage(device, texStaging, nullptr);
        texStaging = VK_NULL_HANDLE;
    }

    if (texStagingMem) {
        devFuncs->vkFreeMemory(device, texStagingMem, nullptr);
        texStagingMem = VK_NULL_HANDLE;
    }

    if (texView) {
        devFuncs->vkDestroyImageView(device, texView, nullptr);
        texView = VK_NULL_HANDLE;
    }

    if (texImage) {
        devFuncs->vkDestroyImage(device, texImage, nullptr);
        texImage = VK_NULL_HANDLE;
    }

    if (texMem) {
        devFuncs->vkFreeMemory(device, texMem, nullptr);
        texMem = VK_NULL_HANDLE;
    }

    if (pipeline) {
        devFuncs->vkDestroyPipeline(device, pipeline, nullptr);
        pipeline = VK_NULL_HANDLE;
    }

    if (pipelineLayout) {
        devFuncs->vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        pipelineLayout = VK_NULL_HANDLE;
    }

    if (pipelineCache) {
        devFuncs->vkDestroyPipelineCache(device, pipelineCache, nullptr);
        pipelineCache = VK_NULL_HANDLE;
    }

    if (descSetLayout) {
        devFuncs->vkDestroyDescriptorSetLayout(device, descSetLayout, nullptr);
        descSetLayout = VK_NULL_HANDLE;
    }

    if (descPool) {
        devFuncs->vkDestroyDescriptorPool(device, descPool, nullptr);
        descPool = VK_NULL_HANDLE;
    }

    if (buf) {
        devFuncs->vkDestroyBuffer(device, buf, nullptr);
        buf = VK_NULL_HANDLE;
    }

    if (bufMem) {
        devFuncs->vkFreeMemory(device, bufMem, nullptr);
        bufMem = VK_NULL_HANDLE;
    }

    if ( computeRenderTargetView ) {
        devFuncs->vkDestroyImageView(device, computeRenderTargetView, nullptr);
        computeRenderTargetView = VK_NULL_HANDLE;
    }

    if ( computeRenderTarget ) {
        devFuncs->vkDestroyImage(device, computeRenderTarget, nullptr);
        computeRenderTarget = VK_NULL_HANDLE;
    }

    if ( computeRenderTargetMemory ) {
        devFuncs->vkFreeMemory(device, computeRenderTargetMemory, nullptr);
        computeRenderTarget = VK_NULL_HANDLE;
    }

    if ( computeDescriptorSetLayout ) {
        devFuncs->vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayout, nullptr);
        computeDescriptorSetLayout = VK_NULL_HANDLE;
    }

    if ( computePipeline ) {
        devFuncs->vkDestroyPipeline(device, computePipeline, nullptr);
        computePipeline = VK_NULL_HANDLE;
    }

    if ( computePipelineLayout ) {
        devFuncs->vkDestroyPipelineLayout(device, computePipelineLayout, nullptr);
        computePipelineLayout = VK_NULL_HANDLE;
    }

    if ( compute.fence ) {
        devFuncs->vkDestroyFence(device, compute.fence, nullptr);
        compute.fence = VK_NULL_HANDLE;
    }

    if ( compute.commandPool )
    {
        VkCommandBuffer buffers[2]=
        {
            compute.commandBuffer,
            compute.commandBufferInit,
        };

        devFuncs->vkFreeCommandBuffers(device, compute.commandPool, 2, &buffers[0]);
        devFuncs->vkDestroyCommandPool(device, compute.commandPool, nullptr);
    }
}

VulkanRenderer::~VulkanRenderer()
{
    qDebug("------------------> Destroying VulkanRenderer");
}
