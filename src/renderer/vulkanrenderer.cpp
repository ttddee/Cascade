/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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
#include <QMouseEvent>
#include <QVulkanWindowRenderer>

#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/color.h>

#include "../vulkanwindow.h"
#include "../uientities/fileboxentity.h"
#include "../benchmark.h"
#include "../multithreading.h"
#include "../log.h"

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
    /// Get device and functions
    device = window->device();
    physicalDevice = window->physicalDevice();

    /// Init all the permanent parts of the renderer
    createVertexBuffer();
    createSampler();
    createDescriptorPool();
    createGraphicsDescriptors();
    createGraphicsPipelineCache();
    createGraphicsPipelineLayout();

    createGraphicsPipeline(graphicsPipelineRGB, ":/shaders/texture_frag.spv");
    createGraphicsPipeline(graphicsPipelineAlpha, ":/shaders/texture_alpha_frag.spv");

    createComputeDescriptors();
    createComputePipelineLayout();

    /// Load all the shaders we need and create their pipelines
    loadShadersFromDisk();
    // Create Noop pipeline
    computePipelineNoop = createComputePipelineNoop();
    // Create a pipeline for each shader
    createComputePipelines();

//    createComputeQueue();
//    createComputeCommandPool();
//    createComputeCommandBuffers();
    computeCommandBuffer = std::unique_ptr<CsCommandBuffer>(
                new CsCommandBuffer(&device,
                                    &physicalDevice,
                                    &(*computePipelineLayout),
                                    &(*computeDescriptorSet)));

    settingsBuffer = std::unique_ptr<CsSettingsBuffer>(new CsSettingsBuffer(
                &device,
                &physicalDevice));

    // Load OCIO config
    try
    {
        const char* file = "ocio/config.ocio";
        ocioConfig = OCIO::Config::CreateFromFile(file);
    }
    catch(OCIO::Exception& exception)
    {
        CS_LOG_CRITICAL("OpenColorIO Error: " + QString(exception.what()));
    }

    emit window->rendererHasBeenCreated();
}

QString VulkanRenderer::getGpuName()
{
    vk::PhysicalDeviceProperties deviceProps = physicalDevice.getProperties();
    auto deviceName = QString::fromLatin1(deviceProps.deviceName);

    return deviceName;
}

void VulkanRenderer::createVertexBuffer()
{
    const vk::PhysicalDeviceLimits pdevLimits(physicalDevice.getProperties().limits);
    const vk::DeviceSize uniAlign = pdevLimits.minUniformBufferOffsetAlignment;

    const vk::DeviceSize vertexAllocSize = aligned(sizeof(vertexData), uniAlign);
    const vk::DeviceSize uniformAllocSize = aligned(UNIFORM_DATA_SIZE, uniAlign);

    vk::BufferCreateInfo bufferInfo({},
                         vertexAllocSize + concurrentFrameCount * uniformAllocSize,
                         vk::BufferUsageFlags(
                                        vk::BufferUsageFlagBits::eVertexBuffer |
                                        vk::BufferUsageFlagBits::eUniformBuffer));
    vertexBuffer = device.createBufferUnique(bufferInfo);

    vk::MemoryRequirements memReq = device.getBufferMemoryRequirements(*vertexBuffer);

    vk::MemoryAllocateInfo memAllocInfo(memReq.size, window->hostVisibleMemoryIndex());

    vertexBufferMemory = device.allocateMemoryUnique(memAllocInfo);

    device.bindBufferMemory(*vertexBuffer, *vertexBufferMemory, 0);

    quint8 *p;
    vk::Result err = device.mapMemory(*vertexBufferMemory,
                     0,
                     memReq.size,
                     {},
                     reinterpret_cast<void **>(&p));

    if (err != vk::Result::eSuccess)
    {
        CS_LOG_WARNING("Failed to map memory for vertex buffer.");
        CS_LOG_CONSOLE("Failed to map memory for vertex buffer.");
    }

    memcpy(p, vertexData, sizeof(vertexData));
    QMatrix4x4 ident;
    //memset(uniformBufferInfo, 0, sizeof(uniformBufferInfo));
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        const vk::DeviceSize offset = vertexAllocSize + i * uniformAllocSize;
        memcpy(p + offset, ident.constData(), 16 * sizeof(float));
        uniformBufferInfo[i].setBuffer(*vertexBuffer);
        uniformBufferInfo[i].setOffset(offset);
        uniformBufferInfo[i].setRange(uniformAllocSize);
    }
    device.unmapMemory(*vertexBufferMemory);
}

void VulkanRenderer::createSampler()
{
    // Create sampler
    vk::SamplerCreateInfo samplerInfo({},
                                      vk::Filter::eNearest,
                                      vk::Filter::eNearest,
                                      vk::SamplerMipmapMode::eNearest,
                                      vk::SamplerAddressMode::eClampToEdge,
                                      vk::SamplerAddressMode::eClampToEdge,
                                      vk::SamplerAddressMode::eClampToEdge,
                                      0,
                                      0);

    sampler = device.createSamplerUnique(samplerInfo);
}

void VulkanRenderer::createDescriptorPool()
{
    // Create descriptor pool
    std::vector<vk::DescriptorPoolSize> descPoolSizes = {
        { vk::DescriptorType::eUniformBuffer,         3 * uint32_t(concurrentFrameCount) },
        { vk::DescriptorType::eCombinedImageSampler,  1 * uint32_t(concurrentFrameCount) },
        { vk::DescriptorType::eCombinedImageSampler,  1 * uint32_t(concurrentFrameCount) },
        { vk::DescriptorType::eStorageImage,          6 * uint32_t(concurrentFrameCount) }
    };

    vk::DescriptorPoolCreateInfo descPoolInfo(
                {},
                6,
                4,
                descPoolSizes.data());

    descriptorPool = device.createDescriptorPoolUnique(descPoolInfo);
}

void VulkanRenderer::createGraphicsDescriptors()
{
    // Create DescriptorSetLayout
    std::vector<vk::DescriptorSetLayoutBinding> layoutBinding = {
        {
            0, // binding
            vk::DescriptorType::eUniformBuffer,
            1, // descriptorCount
            vk::ShaderStageFlagBits::eVertex
        },
        {
            1, // binding
            vk::DescriptorType::eCombinedImageSampler,
            1, // descriptorCount
            vk::ShaderStageFlagBits::eFragment
        }
    };

    vk::DescriptorSetLayoutCreateInfo descLayoutInfo(
                {},
                2, // bindingCount
                layoutBinding.data());

    graphicsDescriptorSetLayout = device.createDescriptorSetLayoutUnique(descLayoutInfo);
}

void VulkanRenderer::createGraphicsPipelineCache()
{
    // Pipeline cache
    vk::PipelineCacheCreateInfo pipelineCacheInfo({}, {});

    pipelineCache = device.createPipelineCacheUnique(pipelineCacheInfo);
}

void VulkanRenderer::createGraphicsPipelineLayout()
{
    vk::PushConstantRange pushConstantRange;
    pushConstantRange.stageFlags                = vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset                    = 0;
    pushConstantRange.size                      = sizeof(viewerPushConstants);

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
                {},
                1,
                &(*graphicsDescriptorSetLayout),
                1,
                &pushConstantRange);

    graphicsPipelineLayout = device.createPipelineLayoutUnique(pipelineLayoutInfo);
}

//void VulkanRenderer::createBuffer(
//        vk::UniqueBuffer& buffer,
//        vk::UniqueDeviceMemory& bufferMemory,
//        vk::DeviceSize& size)
//{
//    vk::BufferCreateInfo bufferInfo(
//                {},
//                size,
//                vk::BufferUsageFlags(
//                    vk::BufferUsageFlagBits::eTransferDst |
//                    vk::BufferUsageFlagBits::eUniformBuffer),
//                vk::SharingMode::eExclusive);

//    buffer = device.createBufferUnique(bufferInfo);

//    vk::MemoryRequirements memRequirements = device.getBufferMemoryRequirements(*buffer);

//    uint32_t memoryType = findMemoryType(
//                memRequirements.memoryTypeBits,
//                vk::MemoryPropertyFlags(
//                    vk::MemoryPropertyFlagBits::eHostVisible |
//                    vk::MemoryPropertyFlagBits::eHostCoherent));

//    vk::MemoryAllocateInfo allocInfo(memRequirements.size,
//                                     memoryType);

//    bufferMemory = device.allocateMemoryUnique(allocInfo);

//    device.bindBufferMemory(*buffer, *bufferMemory, 0);
//}

void VulkanRenderer::fillSettingsBuffer(const NodeBase* node)
{
    auto props = node->getAllPropertyValues();

    settingsBuffer->fillBuffer(props);
}

void VulkanRenderer::createGraphicsPipeline(vk::UniquePipeline& pl,
                                            const QString& fragShaderPath)
{
    // Vertex shader never changes
    vk::UniqueShaderModule vertShaderModule = createShaderFromFile(":/shaders/texture_vert.spv");

    vk::UniqueShaderModule fragShaderModule = createShaderFromFile(fragShaderPath);

    // Graphics pipeline
    vk::GraphicsPipelineCreateInfo pipelineInfo;

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages =
    {
        {
            {},
            vk::ShaderStageFlagBits::eVertex,
            *vertShaderModule,
            "main"
        },
        {
            {},
            vk::ShaderStageFlagBits::eFragment,
            *fragShaderModule,
            "main"
        }
    };
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages.data();

    // Vertex binding
    vk::VertexInputBindingDescription vertexBindingDesc(
                0,
                5 * sizeof(float),
                vk::VertexInputRate::eVertex);

    std::vector<vk::VertexInputAttributeDescription> vertexAttrDesc =
    {
        { // position
            0, // location
            0, // binding
            vk::Format::eR32G32B32Sfloat,
            0
        },
        { // texcoord
            1,
            0,
            vk::Format::eR32G32Sfloat,
            3 * sizeof(float)
        }
    };

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
                {},
                1,
                &vertexBindingDesc,
                2,
                vertexAttrDesc.data());

    pipelineInfo.pVertexInputState = &vertexInputInfo;

    vk::PipelineInputAssemblyStateCreateInfo ia({},
                                                vk::PrimitiveTopology::eTriangleStrip);
    pipelineInfo.pInputAssemblyState = &ia;

    // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
    // This way the pipeline does not need to be touched when resizing the window.
    vk::PipelineViewportStateCreateInfo vp(
                {},
                1,
                {},
                1);
    pipelineInfo.pViewportState = &vp;

    vk::PipelineRasterizationStateCreateInfo rs(
                {},
                false,
                false,
                vk::PolygonMode::eFill,
                vk::CullModeFlagBits::eBack,
                vk::FrontFace::eClockwise,
                {},
                {},
                {},
                {},
                1.0f);
    pipelineInfo.pRasterizationState = &rs;

    vk::PipelineMultisampleStateCreateInfo ms(
                {},
                vk::SampleCountFlagBits::e1);
    pipelineInfo.pMultisampleState = &ms;

    vk::PipelineDepthStencilStateCreateInfo ds(
                {},
                true,
                true,
                vk::CompareOp::eLessOrEqual);
    pipelineInfo.pDepthStencilState = &ds;

    // assume pre-multiplied alpha, blend, write out all of rgba
    vk::PipelineColorBlendAttachmentState att(
                true,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eOne,
                vk::BlendOp::eAdd,
                vk::BlendFactor::eOne,
                vk::BlendFactor::eOne,
                vk::BlendOp::eAdd);

    vk::PipelineColorBlendStateCreateInfo cb(
                {},
                {},
                {},
                1,
                &att);
    pipelineInfo.pColorBlendState = &cb;

    vk::DynamicState dynEnable[] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };

    vk::PipelineDynamicStateCreateInfo dyn({},
                                           sizeof(dynEnable) / sizeof(vk::DynamicState),
                                           dynEnable);
    pipelineInfo.pDynamicState = &dyn;

    pipelineInfo.layout = *graphicsPipelineLayout;
    pipelineInfo.renderPass = window->defaultRenderPass();

    pl = device.createGraphicsPipelineUnique(*pipelineCache, pipelineInfo).value;
}

vk::UniqueShaderModule VulkanRenderer::createShaderFromFile(const QString &name)
{
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly))
    {
        CS_LOG_WARNING("Failed to read shader:");
        CS_LOG_WARNING(qPrintable(name));
    }
    QByteArray blob = file.readAll();
    file.close();

    vk::ShaderModuleCreateInfo shaderInfo(
                {},
                blob.size(),
                reinterpret_cast<const uint32_t *>(blob.constData()));

    vk::UniqueShaderModule shaderModule = device.createShaderModuleUnique(shaderInfo);

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
    // Previous image will be destroyed, so we wait here
    computeCommandBuffer->getQueue()->waitIdle();

    try
    {
        computeRenderTarget = std::unique_ptr<CsImage>(
                    new CsImage(window, &device, &physicalDevice, width, height));
    }
    catch (std::exception const &e)
    {
        CS_LOG_WARNING("Could not create compute render target.");
        return false;
    }

    emit window->renderTargetHasBeenCreated(width, height);

    currentRenderSize = QSize(width, height);

    return true;
}

bool VulkanRenderer::createImageFromFile(const QString &path, const int colorSpace)
{
    cpuImage = std::unique_ptr<ImageBuf>(new ImageBuf(path.toStdString()));
    bool ok = cpuImage->read(0, 0, 0, 4, true, TypeDesc::FLOAT);
    if (!ok)
    {
        CS_LOG_WARNING("There was a problem reading the image from disk.");
        CS_LOG_WARNING(QString::fromStdString(cpuImage->geterror()));
    }

    // Add alpha channel if it doesn't exist
    if (cpuImage->nchannels() == 3)
    {
        int channelorder[] = { 0, 1, 2, -1 };
        float channelvalues[] = { 0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0 };
        std::string channelnames[] = { "R", "G", "B", "A" };

        *cpuImage = ImageBufAlgo::channels(*cpuImage, 4, channelorder, channelvalues, channelnames);
    }

    transformColorSpace(lookupColorSpace(colorSpace), "linear", *cpuImage);

    updateVertexData(cpuImage->xend(), cpuImage->yend());

    // The image we are going to copy into
    // TODO: We can skip this and copy straight into cache
    tmpCacheImage = std::unique_ptr<CsImage>(
                new CsImage(window,
                            &device,
                            &physicalDevice,
                            cpuImage->xend(),
                            cpuImage->yend()));

    auto imageSize = QSize(cpuImage->xend(), cpuImage->yend());

    // Now we can either map and copy the image data directly, or have to go
    // through a staging buffer to copy and convert into the internal optimal
    // tiling format.
    vk::FormatProperties props = physicalDevice.getFormatProperties(globalImageFormat);
    const bool canSampleLinear = (bool)(props.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);
    const bool canSampleOptimal = (bool)(props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);
    if (!canSampleLinear && !canSampleOptimal) {
        CS_LOG_WARNING("Neither linear nor optimal image sampling is supported for image");
        return false;
    }

    // The image that gets the data from the CPU
    loadImageStaging = std::unique_ptr<CsImage>(new CsImage(
                                                    window,
                                                    &device,
                                                    &physicalDevice,
                                                    cpuImage->xend(),
                                                    cpuImage->yend(),
                                                    true));

    if (!writeLinearImage(
                static_cast<float*>(cpuImage->localpixels()),
                QSize(cpuImage->xend(), cpuImage->yend()),
                loadImageStaging))
    {
        CS_LOG_WARNING("Failed to write linear image");
        CS_LOG_CONSOLE("Failed to write linear image");
        return false;
    }


    computeCommandBuffer->setTexStagingPending(true);

    loadImageSize = imageSize;

    return true;
}

QString VulkanRenderer::lookupColorSpace(const int i)
{
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

    switch(i)
    {
        case 0:
            return "sRGB";
        case 2:
            return "rec709";
        case 3:
            return "Gamma1.8";
        case 4:
            return "Gamma2.2";
        case 5:
            return "Panalog";
        case 6:
            return "REDLog";
        case 7:
            return "ViperLog";
        case 8:
            return "AlexaV3LogC";
        case 9:
            return "PLogLin";
        case 10:
            return "SLog";
        case 11:
            return "raw";
        default:
            return "linear";
    }
}

void VulkanRenderer::transformColorSpace(const QString& from, const QString& to, ImageBuf& image)
{
    parallelApplyColorSpace(
                ocioConfig,
                from,
                to,
                static_cast<float*>(image.localpixels()),
                image.xend(),
                image.yend());
}

void VulkanRenderer::createComputeDescriptors()
{
    // TODO: Clean this up.

    if (!computeDescriptorSetLayout)
    {
        // Define the layout of the input of the shader.
        // 2 images to read, 1 image to write
        std::vector<vk::DescriptorSetLayoutBinding> bindings(4);

        bindings.at(0).binding         = 0;
        bindings.at(0).descriptorType  = vk::DescriptorType::eStorageImage;
        bindings.at(0).descriptorCount = 1;
        bindings.at(0).stageFlags      = vk::ShaderStageFlagBits::eCompute;

        bindings.at(1).binding         = 1;
        bindings.at(1).descriptorType  = vk::DescriptorType::eStorageImage;
        bindings.at(1).descriptorCount = 1;
        bindings.at(1).stageFlags      = vk::ShaderStageFlagBits::eCompute;

        bindings.at(2).binding         = 2;
        bindings.at(2).descriptorType  = vk::DescriptorType::eStorageImage;
        bindings.at(2).descriptorCount = 1;
        bindings.at(2).stageFlags      = vk::ShaderStageFlagBits::eCompute;

        bindings.at(3).binding         = 3;
        bindings.at(3).descriptorType  = vk::DescriptorType::eUniformBuffer;
        bindings.at(3).descriptorCount = 1;
        bindings.at(3).stageFlags      = vk::ShaderStageFlagBits::eCompute;

        vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo(
                    {},
                    4,
                    &bindings.at(0));

        computeDescriptorSetLayout = device.createDescriptorSetLayoutUnique(
                    descSetLayoutCreateInfo);
    }

    graphicsDescriptorSet.reserve(2);

    // Descriptor sets
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        {
            vk::DescriptorSetAllocateInfo descSetAllocInfo(
                        *descriptorPool,
                        1,
                        &(*graphicsDescriptorSetLayout));

            graphicsDescriptorSet.push_back(std::move(device.allocateDescriptorSetsUnique(descSetAllocInfo).front()));
        }
    }

    vk::DescriptorSetAllocateInfo descSetAllocInfoCompute(
                *descriptorPool,
                1,
                &(*computeDescriptorSetLayout));

    computeDescriptorSet = std::move(device.allocateDescriptorSetsUnique(descSetAllocInfoCompute).front());
}

void VulkanRenderer::updateComputeDescriptors(
        const CsImage* const inputImageBack,
        const CsImage* const inputImageFront,
        const CsImage* const outputImage)
{
    for (int i = 0; i < concurrentFrameCount; ++i)
    {
        std::vector<vk::WriteDescriptorSet> descWrite(2);
        descWrite.at(0).dstSet = *graphicsDescriptorSet.at(i);
        descWrite.at(0).dstBinding = 0;
        descWrite.at(0).descriptorCount = 1;
        descWrite.at(0).descriptorType = vk::DescriptorType::eUniformBuffer;
        descWrite.at(0).pBufferInfo = &uniformBufferInfo[i];

        vk::DescriptorImageInfo descImageInfo(
                    *sampler,
                    *outputImage->getImageView(),
                    vk::ImageLayout::eShaderReadOnlyOptimal);

        descWrite.at(1).dstSet = *graphicsDescriptorSet.at(i);
        descWrite.at(1).dstBinding = 1;
        descWrite.at(1).descriptorCount = 1;
        descWrite.at(1).descriptorType = vk::DescriptorType::eCombinedImageSampler;
        descWrite.at(1).pImageInfo = &descImageInfo;

        device.updateDescriptorSets(descWrite, {});
    }

    {
        vk::DescriptorImageInfo sourceInfoBack(
                    *sampler,
                    *inputImageBack->getImageView(),
                    vk::ImageLayout::eGeneral);

        vk::DescriptorImageInfo sourceInfoFront;
        sourceInfoFront.sampler = *sampler;
        if (inputImageFront)
            sourceInfoFront.imageView = *inputImageFront->getImageView();
        else
            sourceInfoFront.imageView = *inputImageBack->getImageView();
        sourceInfoFront.imageLayout = vk::ImageLayout::eGeneral;

        vk::DescriptorImageInfo destinationInfo(
                    {},
                    *outputImage->getImageView(),
                    vk::ImageLayout::eGeneral);

        vk::DescriptorBufferInfo settingsBufferInfo(
                    *settingsBuffer->getBuffer(),
                    0,
                    VK_WHOLE_SIZE);

        std::vector<vk::WriteDescriptorSet> descWrite(4);

        descWrite.at(0).dstSet                    = *computeDescriptorSet;
        descWrite.at(0).dstBinding                = 0;
        descWrite.at(0).descriptorCount           = 1;
        descWrite.at(0).descriptorType            = vk::DescriptorType::eStorageImage;
        descWrite.at(0).pImageInfo                = &sourceInfoBack;

        descWrite.at(1).dstSet                    = *computeDescriptorSet;
        descWrite.at(1).dstBinding                = 1;
        descWrite.at(1).descriptorCount           = 1;
        descWrite.at(1).descriptorType            = vk::DescriptorType::eStorageImage;
        descWrite.at(1).pImageInfo                = &sourceInfoFront;

        descWrite.at(2).dstSet                    = *computeDescriptorSet;
        descWrite.at(2).dstBinding                = 2;
        descWrite.at(2).descriptorCount           = 1;
        descWrite.at(2).descriptorType            = vk::DescriptorType::eStorageImage;
        descWrite.at(2).pImageInfo                = &destinationInfo;

        descWrite.at(3).dstSet                    = *computeDescriptorSet;
        descWrite.at(3).dstBinding                = 3;
        descWrite.at(3).descriptorCount           = 1;
        descWrite.at(3).descriptorType            = vk::DescriptorType::eUniformBuffer;
        descWrite.at(3).pBufferInfo               = &settingsBufferInfo;

        device.updateDescriptorSets(descWrite, {});
    }
}

void VulkanRenderer::createComputePipelineLayout()
{
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
                {},
                1,
                &(*computeDescriptorSetLayout));

    //Create the layout, store it to share between shaders
    computePipelineLayout = device.createPipelineLayoutUnique(pipelineLayoutInfo);
}


void VulkanRenderer::createComputePipelines()
{
    for (int i = 0; i != NODE_TYPE_MAX; i++)
    {
        NodeType nodeType = static_cast<NodeType>(i);

        pipelines[nodeType] = createComputePipeline(nodeType);
    }
}

vk::UniquePipeline VulkanRenderer::createComputePipeline(NodeType nodeType)
{
    auto shaderModule = *shaders[nodeType];

    vk::PipelineShaderStageCreateInfo computeStage(
                {},
                vk::ShaderStageFlagBits::eCompute,
                shaderModule,
                "main");

    vk::ComputePipelineCreateInfo pipelineInfo(
                {},
                computeStage,
                *computePipelineLayout);

    vk::UniquePipeline pl = device.createComputePipelineUnique(*pipelineCache, pipelineInfo).value;

    return pl;
}

vk::UniquePipeline VulkanRenderer::createComputePipelineNoop()
{
    // TODO: This should not need its own function
    auto shaderModule = createShaderFromFile(":/shaders/noop_comp.spv");;

    vk::PipelineShaderStageCreateInfo computeStage(
                {},
                vk::ShaderStageFlagBits::eCompute,
                *shaderModule,
                "main");

    vk::ComputePipelineCreateInfo pipelineInfo(
                {},
                computeStage,
                *computePipelineLayout);

    vk::UniquePipeline pl = device.createComputePipelineUnique(*pipelineCache, pipelineInfo).value;

    return pl;
}

void VulkanRenderer::createQueryPool()
{
    vk::QueryPoolCreateInfo queryPoolInfo(
                {},
                vk::QueryType::eTimestamp,
                2);

    queryPool = device.createQueryPoolUnique(queryPoolInfo);
}

bool VulkanRenderer::writeLinearImage(
        float* imgStart,
        QSize imgSize,
        std::unique_ptr<CsImage>& image)
{
    vk::ImageSubresource subres(
                vk::ImageAspectFlagBits::eColor,
                0, // mip level
                0);

    vk::SubresourceLayout layout = device.getImageSubresourceLayout(*image->getImage(), subres);

    float *p;
    vk::Result err = device.mapMemory(*image->getMemory(),
                                      layout.offset,
                                      layout.size,
                                      {},
                                      reinterpret_cast<void **>(&p));
    if (err != vk::Result::eSuccess) {
        CS_LOG_WARNING("Failed to map memory for linear image.");
        CS_LOG_CONSOLE("Failed to map memory for linear image.");
        return false;
    }

    int pad = (layout.rowPitch - imgSize.width() * 16) / 4;

    // TODO: Parallelize this
    float* pixels = imgStart;
    int lineWidth = imgSize.width() * 16; // 4 channels * 4 bytes
    for (int y = 0; y < imgSize.height(); ++y)
    {
        memcpy(p, pixels, lineWidth);
        pixels += imgSize.width() * 4;
        p += imgSize.width() * 4 + pad;
    }

    device.unmapMemory(*image->getMemory());

    return true;
}

void VulkanRenderer::updateVertexData(const int w, const int h)
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
    CS_LOG_INFO("Initializing swapchain resources.");

    // Projection matrix
    projection = window->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
    const QSize sz = window->swapChainImageSize();
    projection.ortho( -sz.width() / scaleXY, sz.width() / scaleXY, -sz.height() / scaleXY, sz.height() / scaleXY, -1.0f, 100.0f);
    projection.scale(500);
}

void VulkanRenderer::setDisplayMode(const DisplayMode mode)
{
    displayMode = mode;
}

bool VulkanRenderer::saveImageToDisk(
        CsImage* const inputImage,
        const QString &path,
        const int colorSpace)
{
    bool success = true;

    auto mem = computeCommandBuffer->recordImageSave(
                inputImage);

    computeCommandBuffer->submitImageSave();

    device.waitIdle();

    float *pInput;
    device.mapMemory(
                *mem,
                0,
                VK_WHOLE_SIZE,
                {},
                reinterpret_cast<void **>(&pInput));

    int width = inputImage->getWidth();
    int height = inputImage->getHeight();
    int numValues = width * height * 4;

    float* output = new float[numValues];
    float* pOutput = &output[0];

    parallelArrayCopy(pInput, pOutput, width, height);

    ImageSpec spec(width, height, 4, TypeDesc::FLOAT);
    std::unique_ptr<ImageBuf> saveImage =
            std::unique_ptr<ImageBuf>(new ImageBuf(spec, output));

    transformColorSpace("linear", lookupColorSpace(colorSpace), *saveImage);

    success = saveImage->write(path.toStdString());

    if (!success)
    {
        CS_LOG_INFO("Problem saving image." + QString::fromStdString(saveImage->geterror()));
    }

    delete[] output;

    device.unmapMemory(*mem);

    return success;
}

void VulkanRenderer::createRenderPass()
{
    CS_LOG_INFO("Creating Render Pass.");

    vk::CommandBuffer* cb = computeCommandBuffer->getGeneric();

    vk::CommandBufferBeginInfo cbBegin;
    cb->begin(cbBegin);

    const QSize sz = window->swapChainImageSize();

    // Clear background
    vk::ClearDepthStencilValue clearDS = { 1, 0 };
    vk::ClearValue clearValues[2];
    clearValues[0].color = clearColor;
    clearValues[1].depthStencil = clearDS;

    vk::RenderPassBeginInfo rpBeginInfo;
    rpBeginInfo.renderPass = window->defaultRenderPass();
    rpBeginInfo.framebuffer = window->currentFramebuffer();
    rpBeginInfo.renderArea.extent.width = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount = 2;
    rpBeginInfo.pClearValues = clearValues;
    cb->beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    // TODO: Can we do this once?
    quint8 *p;
    vk::Result err = device.mapMemory(
                *vertexBufferMemory,
                uniformBufferInfo[window->currentFrame()].offset,
                UNIFORM_DATA_SIZE,
                {},
                reinterpret_cast<void **>(&p));

    if (err != vk::Result::eSuccess)
    {
        CS_LOG_WARNING("Failed to map memory for vertex buffer.");
    }

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
    device.unmapMemory(*vertexBufferMemory);

    // Choose to either display RGB or Alpha
    vk::Pipeline pl;
    if (displayMode == DISPLAY_MODE_ALPHA)
        pl = *graphicsPipelineAlpha;
    else
        pl = *graphicsPipelineRGB;

    cb->pushConstants(
                *graphicsPipelineLayout,
                vk::ShaderStageFlagBits::eFragment,
                0,
                sizeof(viewerPushConstants),
                viewerPushConstants.data());
    cb->bindPipeline(
                vk::PipelineBindPoint::eGraphics,
                pl);
    cb->bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics,
                *graphicsPipelineLayout,
                0,
                *graphicsDescriptorSet.at(window->currentFrame()),
                {});

    vk::DeviceSize vbOffset = 0;
    cb->bindVertexBuffers(
                0,
                1,
                &(*vertexBuffer),
                &vbOffset);

    //negative viewport
    vk::Viewport viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = sz.width();
    viewport.height = sz.height();
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    cb->setViewport(
                0,
                1,
                &viewport);

    vk::Rect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width = viewport.width;
    scissor.extent.height = viewport.height;
    cb->setScissor(
                0,
                1,
                &scissor);

    cb->draw(4, 1, 0, 0);

    cb->endRenderPass();
    cb->end();
}

//void VulkanRenderer::submitComputeCommands()
//{
//    // Submit compute commands
//    // Use a fence to ensure that compute command buffer has finished executing before using it again
//    device.waitForFences(1, &(*compute.fence), true, UINT64_MAX);
//    device.resetFences(1, &(*compute.fence));

//    // Do the copy on the compute queue
//    vk::SubmitInfo computeSubmitInfo;
//    computeSubmitInfo.commandBufferCount = 1;

//    if (texStagingPending)
//    {
//        texStagingPending = false;
//        computeSubmitInfo.pCommandBuffers = &(*compute.commandBufferImageLoad);
//    }
//    else
//    {
//        computeSubmitInfo.pCommandBuffers = &(*compute.commandBufferGeneric);
//    }
//    compute.computeQueue.submit(
//                1,
//                &computeSubmitInfo,
//                *compute.fence);
//}

//void VulkanRenderer::submitImageSaveCommand()
//{
//    device.waitForFences(1, &(*compute.fence), true, UINT64_MAX);
//    device.resetFences(1, &(*compute.fence));

//    vk::SubmitInfo computeSubmitInfo;
//    computeSubmitInfo.commandBufferCount = 1;
//    computeSubmitInfo.pCommandBuffers = &(*compute.commandBufferImageSave);

//    compute.computeQueue.submit(
//                1,
//                &computeSubmitInfo,
//                *compute.fence);
//}

std::vector<float> VulkanRenderer::unpackPushConstants(const QString& s)
{
    // TODO: Move into render utility
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
    viewerPushConstants = unpackPushConstants(s);
}

void VulkanRenderer::processReadNode(NodeBase *node)
{
    auto parts = node->getAllPropertyValues().split(",");
    int index = parts[parts.size() - 2].toInt();
    if ( index < 0 )
        index = 0;
    QString path = parts[index];
    int colorSpace = parts.last().toInt();

    if(path != "")
    {
        imagePath = path;

        // Create texture
        if (!createImageFromFile(imagePath, colorSpace))
            CS_LOG_WARNING("Failed to create texture");

        // Update the projection size
        createVertexBuffer();

        // Create render target
        if (!createComputeRenderTarget(cpuImage->xend(), cpuImage->yend()))
            CS_LOG_WARNING("Failed to create compute render target.");

        updateComputeDescriptors(&(*tmpCacheImage), nullptr, &(*computeRenderTarget));

        computeCommandBuffer->recordImageLoad(
                    &(*loadImageStaging),
                    &(*tmpCacheImage),
                    &(*computeRenderTarget),
                    &(*pipelines[NODE_TYPE_READ]));

        computeCommandBuffer->submitGeneric();

        CS_LOG_INFO("Moving render target.");

        node->setCachedImage(std::move(computeRenderTarget));
    }
}

void VulkanRenderer::processNode(
        NodeBase* node,
        CsImage* inputImageBack,
        CsImage* inputImageFront,
        const QSize targetSize)
{

    fillSettingsBuffer(node);

    if (currentRenderSize != targetSize)
    {
        updateVertexData(targetSize.width(), targetSize.height());
        createVertexBuffer();
    }

    if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
        qFatal("Failed to create compute render target.");

    // Tells the shader if we have a mask on the front input
    settingsBuffer->appendValue(0.0);
    if (inputImageFront)
    {
        settingsBuffer->incrementLastValue();
    }

    // TODO: This is a workaround for generative nodes without input
    // but needs to be fixed
    if (!inputImageBack)
    {
        tmpCacheImage = std::unique_ptr<CsImage>(
                    new CsImage(window, &device, &physicalDevice,
                                targetSize.width(), targetSize.height()));
        inputImageBack = tmpCacheImage.get();
    }

    int numShaderPasses = getPropertiesForType(node->nodeType).numShaderPasses;
    int currentShaderPass = 1;

    if (numShaderPasses == 1)
    {
        updateComputeDescriptors(inputImageBack, inputImageFront, computeRenderTarget.get());

        computeCommandBuffer->recordGeneric(
                    inputImageBack,
                    inputImageFront,
                    computeRenderTarget.get(),
                    *pipelines[node->nodeType],
                    numShaderPasses,
                    currentShaderPass);

        computeCommandBuffer->submitGeneric();

        window->requestUpdate();

        device.waitIdle();

        node->setCachedImage(std::move(computeRenderTarget));
    }
    else
    {
        for (int i = 1; i <= numShaderPasses; ++i)
        {
            // TODO: Shorten this
            if (currentShaderPass == 1)
            {
                // First pass of multipass shader
                settingsBuffer->appendValue(0.0);

                updateComputeDescriptors(inputImageBack, inputImageFront, computeRenderTarget.get());

                computeCommandBuffer->recordGeneric(
                            inputImageBack,
                            inputImageFront,
                            computeRenderTarget.get(),
                            *pipelines[node->nodeType],
                            numShaderPasses,
                            currentShaderPass);

                computeCommandBuffer->submitGeneric();
            }
            else if (currentShaderPass <= numShaderPasses)
            {
                // Subsequent passes
                settingsBuffer->incrementLastValue();

                if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
                    qFatal("Failed to create compute render target.");

                updateComputeDescriptors(node->getCachedImage(), inputImageFront, computeRenderTarget.get());

                computeCommandBuffer->recordGeneric(
                            node->getCachedImage(),
                            inputImageFront,
                            computeRenderTarget.get(),
                            *pipelines[node->nodeType],
                            numShaderPasses,
                            currentShaderPass);

                computeCommandBuffer->submitGeneric();
            }
            currentShaderPass++;

            device.waitIdle();

            node->setCachedImage(std::move(computeRenderTarget));
        }
        window->requestUpdate();
    }
}

void VulkanRenderer::displayNode(const NodeBase *node)
{
    // TODO: Should probably use something like cmdBlitImage
    // instead of the hacky noop shader workaround
    // for displaying a node that has already been rendered
    if(CsImage* image = node->getCachedImage())
    {
        CS_LOG_INFO("Displaying node.");
        clearScreen = false;

        updateVertexData(image->getWidth(), image->getHeight());
        createVertexBuffer();

        if (!createComputeRenderTarget(image->getWidth(), image->getHeight()))
            qFatal("Failed to create compute render target.");

        updateComputeDescriptors(image, nullptr, computeRenderTarget.get());

        computeCommandBuffer->recordGeneric(
                    image,
                    nullptr,
                    computeRenderTarget.get(),
                    *computePipelineNoop,
                    1,
                    1);

        computeCommandBuffer->submitGeneric();

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
    CS_LOG_INFO("Starting next frame.");

    if (clearScreen)
    {
        const QSize sz = window->swapChainImageSize();

        // Clear background
        vk::ClearDepthStencilValue clearDS = { 1, 0 };
        vk::ClearValue clearValues[2];
        memset(clearValues, 0, sizeof(clearValues));
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = clearDS;

        vk::RenderPassBeginInfo rpBeginInfo;
        rpBeginInfo.renderPass = window->defaultRenderPass();
        rpBeginInfo.framebuffer = window->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount = 2;
        rpBeginInfo.pClearValues = clearValues;
        vk::CommandBuffer cmdBuf = window->currentCommandBuffer();
        cmdBuf.beginRenderPass(
                    &rpBeginInfo,
                    vk::SubpassContents::eInline);

        cmdBuf.endRenderPass();
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
    emit window->requestZoomTextUpdate(s);
}

void VulkanRenderer::releaseSwapChainResources()
{
    device.waitIdle();

    CS_LOG_INFO("Releasing resources.");
    loadImageStaging = nullptr;
    tmpCacheImage = nullptr;
    computeRenderTarget = nullptr;
    settingsBuffer = nullptr;
    computeCommandBuffer = nullptr;
    for(auto& pl : pipelines)
        device.destroy(*pl.second);
    device.destroy(*computePipelineNoop);
    for(auto& sh : shaders)
        device.destroy(*sh.second);
    device.destroy(*computePipelineLayout);
    device.destroy(*descriptorPool);
    device.destroy(*graphicsDescriptorSetLayout);
    device.destroy(*computeDescriptorSetLayout);
    device.destroy(*graphicsPipelineLayout);
    device.destroy(*pipelineCache);
    device.destroy(*graphicsPipelineRGB);
    device.destroy(*graphicsPipelineAlpha);
    device.destroy(*sampler);
    device.free(*vertexBufferMemory);
    device.destroy(*vertexBuffer);

    device.waitIdle();
}

void VulkanRenderer::releaseResources()
{

}

VulkanRenderer::~VulkanRenderer()
{
    CS_LOG_INFO("Destroying Renderer.");
}
