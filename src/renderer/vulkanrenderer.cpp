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

#include <QCoreApplication>
#include <QFile>
#include <QMouseEvent>
#include <QVulkanFunctions>
#include <QVulkanWindowRenderer>

#include <OpenImageIO/color.h>
#include <OpenImageIO/imagebufalgo.h>

#include "../benchmark.h"
#include "../log.h"
#include "../multithreading.h"
#include "../uientities/fileboxentity.h"
#include "../vulkanwindow.h"
#include "renderutility.h"

namespace Cascade::Renderer
{

// Use a triangle strip to get a quad.
static float vertexData[] = { // Y up, front = CW
    // x, y, z, u, v
    -1, -1, 0, 0, 1, -1, 1, 0, 0, 0, 1, -1, 0, 1, 1, 1, 1, 0, 1, 0};

VulkanRenderer& VulkanRenderer::getInstance()
{
    static VulkanRenderer instance;

    return instance;
}

void VulkanRenderer::setUp(VulkanWindow* w)
{
    mWindow = w;

    mConcurrentFrameCount = mWindow->concurrentFrameCount();
}

void VulkanRenderer::initResources()
{
    // Get device and functions
    mDevice         = mWindow->device();
    mPhysicalDevice = mWindow->physicalDevice();

    // Init all the permanent parts of the renderer
    createVertexBuffer();
    createSampler();
    createDescriptorPool();
    createGraphicsDescriptors();
    createGraphicsPipelineCache();
    createGraphicsPipelineLayout();

    createGraphicsPipeline(mGraphicsPipelineRGB, ":/shaders/texture_frag.spv");
    createGraphicsPipeline(mGraphicsPipelineAlpha, ":/shaders/texture_alpha_frag.spv");

    createComputeDescriptors();
    createComputePipelineLayout();

    // Load all the shaders we need and create their pipelines
    loadShadersFromDisk();
    // Create Noop pipeline
    mComputePipelineNoop =
        createComputePipeline(createShaderFromFile(":/shaders/noop_comp.spv").get());
    // Create a pipeline for each shader
    createComputePipelines();

    mComputeCommandBuffer = std::unique_ptr<CsCommandBuffer>(new CsCommandBuffer(
        &mDevice, &mPhysicalDevice, &mComputePipelineLayout.get(), &mComputeDescriptorSet.get()));

    mSettingsBuffer =
        std::unique_ptr<CsSettingsBuffer>(new CsSettingsBuffer(&mDevice, &mPhysicalDevice));

    // Load OCIO config
    try
    {
        const char* file = "ocio/config.ocio";
        mOcioConfig      = OCIO::Config::CreateFromFile(file);
    }
    catch (OCIO::Exception& exception)
    {
        CS_LOG_WARNING("OpenColorIO Error: " + QString(exception.what()));
    }

    emit mWindow->rendererHasBeenCreated();
}

QString VulkanRenderer::getGpuName()
{
    vk::PhysicalDeviceProperties deviceProps = mPhysicalDevice.getProperties();
    auto deviceName                          = QString::fromLatin1(deviceProps.deviceName);

    return deviceName;
}

void VulkanRenderer::createVertexBuffer()
{
    // The current vertexBuffer will be destroyed,
    // so we have to wait here.
    auto result = mDevice.waitIdle();

    const vk::PhysicalDeviceLimits pdevLimits(mPhysicalDevice.getProperties().limits);
    const vk::DeviceSize uniAlign = pdevLimits.minUniformBufferOffsetAlignment;

    const vk::DeviceSize vertexAllocSize  = aligned(sizeof(vertexData), uniAlign);
    const vk::DeviceSize uniformAllocSize = aligned(uniformDataSize, uniAlign);

    vk::BufferCreateInfo bufferInfo(
        {},
        vertexAllocSize + mConcurrentFrameCount * uniformAllocSize,
        vk::BufferUsageFlags(
            vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eUniformBuffer));
    mVertexBuffer = mDevice.createBufferUnique(bufferInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
            vk::ObjectType::eBuffer,
            NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkBuffer, *mVertexBuffer),
            "Vertex Buffer");
        result = mDevice.setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
    }
#endif

    vk::MemoryRequirements memReq = mDevice.getBufferMemoryRequirements(*mVertexBuffer);

    vk::MemoryAllocateInfo memAllocInfo(memReq.size, mWindow->hostVisibleMemoryIndex());

    mVertexBufferMemory = mDevice.allocateMemoryUnique(memAllocInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
            vk::ObjectType::eDeviceMemory,
            NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkDeviceMemory, *mVertexBufferMemory),
            "Vertex Buffer Memory");
        result = mDevice.setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
    }
#endif

    // copy the vertex and color data into device memory
    uint8_t* pData =
        static_cast<uint8_t*>(mDevice.mapMemory(mVertexBufferMemory.get(), 0, memReq.size).value);
    memcpy(pData, vertexData, sizeof(vertexData));

    QMatrix4x4 ident;
    for (int i = 0; i < mConcurrentFrameCount; ++i)
    {
        const vk::DeviceSize offset = vertexAllocSize + i * uniformAllocSize;
        memcpy(pData + offset, ident.constData(), 16 * sizeof(float));
        mUniformBufferInfo[i].setBuffer(*mVertexBuffer);
        mUniformBufferInfo[i].setOffset(offset);
        mUniformBufferInfo[i].setRange(uniformAllocSize);
    }
    mDevice.unmapMemory(mVertexBufferMemory.get());

    result = mDevice.bindBufferMemory(*mVertexBuffer, *mVertexBufferMemory, 0);
    Q_UNUSED(result);
}

void VulkanRenderer::createSampler()
{
    // Create sampler
    vk::SamplerCreateInfo samplerInfo(
        {},
        vk::Filter::eNearest,
        vk::Filter::eNearest,
        vk::SamplerMipmapMode::eNearest,
        vk::SamplerAddressMode::eClampToEdge,
        vk::SamplerAddressMode::eClampToEdge,
        vk::SamplerAddressMode::eClampToEdge,
        {},
        false);

    mSampler = mDevice.createSamplerUnique(samplerInfo).value;
}

void VulkanRenderer::createDescriptorPool()
{
    // Create descriptor pool
    std::vector<vk::DescriptorPoolSize> descPoolSizes = {
        {vk::DescriptorType::eUniformBuffer, 3 * uint32_t(mConcurrentFrameCount)},
        {vk::DescriptorType::eCombinedImageSampler, 1 * uint32_t(mConcurrentFrameCount)},
        {vk::DescriptorType::eCombinedImageSampler, 1 * uint32_t(mConcurrentFrameCount)},
        {vk::DescriptorType::eStorageImage, 6 * uint32_t(mConcurrentFrameCount)}};

    vk::DescriptorPoolCreateInfo descPoolInfo({}, 6, 4, descPoolSizes.data());

    mDescriptorPool = mDevice.createDescriptorPoolUnique(descPoolInfo).value;
}

void VulkanRenderer::createGraphicsDescriptors()
{
    // Create DescriptorSetLayout
    std::vector<vk::DescriptorSetLayoutBinding> layoutBinding = {
        {0, // binding
         vk::DescriptorType::eUniformBuffer,
         1, // descriptorCount
         vk::ShaderStageFlagBits::eVertex},
        {1, // binding
         vk::DescriptorType::eCombinedImageSampler,
         1, // descriptorCount
         vk::ShaderStageFlagBits::eFragment},
        {2, // binding
         vk::DescriptorType::eCombinedImageSampler,
         1, // descriptorCount
         vk::ShaderStageFlagBits::eFragment}};

    vk::DescriptorSetLayoutCreateInfo descLayoutInfo(
        {},
        3, // bindingCount
        layoutBinding.data());

    mGraphicsDescriptorSetLayout = mDevice.createDescriptorSetLayoutUnique(descLayoutInfo).value;
}

void VulkanRenderer::createGraphicsPipelineCache()
{
    // Pipeline cache
    vk::PipelineCacheCreateInfo pipelineCacheInfo;
    mPipelineCache = mDevice.createPipelineCacheUnique(pipelineCacheInfo).value;
}

void VulkanRenderer::createGraphicsPipelineLayout()
{
    vk::PushConstantRange pushConstantRange;
    pushConstantRange.stageFlags = vk::ShaderStageFlagBits::eFragment;
    pushConstantRange.offset     = 0;
    pushConstantRange.size       = sizeof(mViewerPushConstants);

    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
        {}, 1, &(*mGraphicsDescriptorSetLayout), 1, &pushConstantRange);

    mGraphicsPipelineLayout = mDevice.createPipelineLayoutUnique(pipelineLayoutInfo).value;
}

void VulkanRenderer::fillSettingsBuffer(const NodeBase* node)
{
    //    auto props = node->getAllPropertyValues();

    //    mSettingsBuffer->fillBuffer(props);
}

void VulkanRenderer::createGraphicsPipeline(vk::UniquePipeline& pl, const QString& fragShaderPath)
{
    // Vertex shader never changes
    vk::UniqueShaderModule vertShaderModule = createShaderFromFile(":/shaders/texture_vert.spv");

    vk::UniqueShaderModule fragShaderModule = createShaderFromFile(fragShaderPath);

    // Graphics pipeline
    vk::GraphicsPipelineCreateInfo pipelineInfo;

    std::vector<vk::PipelineShaderStageCreateInfo> shaderStages = {
        {{}, vk::ShaderStageFlagBits::eVertex, *vertShaderModule, "main"},
        {{}, vk::ShaderStageFlagBits::eFragment, *fragShaderModule, "main"}};
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages    = shaderStages.data();

    // Vertex binding
    vk::VertexInputBindingDescription vertexBindingDesc(
        0, 5 * sizeof(float), vk::VertexInputRate::eVertex);

    std::vector<vk::VertexInputAttributeDescription> vertexAttrDesc(
        {{// position
          0, // location
          0, // binding
          vk::Format::eR32G32B32Sfloat,
          0},
         {// texcoord
          1,
          0,
          vk::Format::eR32G32Sfloat,
          3 * sizeof(float)}});

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        {}, 1, &vertexBindingDesc, 2, vertexAttrDesc.data());

    pipelineInfo.pVertexInputState = &vertexInputInfo;

    vk::PipelineInputAssemblyStateCreateInfo ia({}, vk::PrimitiveTopology::eTriangleStrip);
    pipelineInfo.pInputAssemblyState = &ia;

    // The viewport and scissor will be set dynamically via vkCmdSetViewport/Scissor.
    // This way the pipeline does not need to be touched when resizing the window.
    vk::PipelineViewportStateCreateInfo vp({}, 1, {}, 1);
    pipelineInfo.pViewportState = &vp;

    vk::PipelineRasterizationStateCreateInfo rs(
        {},
        {},
        {},
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eClockwise,
        {},
        {},
        {},
        {},
        1.0f);
    pipelineInfo.pRasterizationState = &rs;

    vk::PipelineMultisampleStateCreateInfo ms({}, vk::SampleCountFlagBits::e1);
    pipelineInfo.pMultisampleState = &ms;

    vk::PipelineDepthStencilStateCreateInfo ds({}, true, true, vk::CompareOp::eLessOrEqual);
    pipelineInfo.pDepthStencilState = &ds;

    // assume pre-multiplied alpha, blend, write out all of rgba
    vk::PipelineColorBlendAttachmentState att(
        true,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eOne,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eOne,
        vk::BlendOp::eAdd,
        {vk::ColorComponentFlags(
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)});

    vk::PipelineColorBlendStateCreateInfo cb({}, {}, {}, 1, &att);
    pipelineInfo.pColorBlendState = &cb;

    vk::DynamicState dynEnable[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    vk::PipelineDynamicStateCreateInfo dyn(
        {}, sizeof(dynEnable) / sizeof(vk::DynamicState), dynEnable);
    pipelineInfo.pDynamicState = &dyn;

    pipelineInfo.layout     = *mGraphicsPipelineLayout;
    pipelineInfo.renderPass = mWindow->defaultRenderPass();

    pl = std::move(mDevice.createGraphicsPipelineUnique(*mPipelineCache, pipelineInfo).value);
}

vk::UniqueShaderModule VulkanRenderer::createShaderFromFile(const QString& name)
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
        {}, blob.size(), reinterpret_cast<const uint32_t*>(blob.constData()));

    vk::UniqueShaderModule shaderModule = mDevice.createShaderModuleUnique(shaderInfo).value;

    return shaderModule;
}

vk::UniqueShaderModule VulkanRenderer::createShaderFromCode(const std::vector<unsigned int>& code)
{
    auto codeChar = uintVecToCharVec(code);

    QByteArray codeArray =
        QByteArray(reinterpret_cast<const char*>(codeChar.data()), codeChar.size());

    vk::ShaderModuleCreateInfo shaderInfo(
        {}, codeArray.size(), reinterpret_cast<const uint32_t*>(codeArray.constData()));

    vk::UniqueShaderModule shaderModule = mDevice.createShaderModuleUnique(shaderInfo).value;

    return shaderModule;
}

void VulkanRenderer::loadShadersFromDisk()
{
    //    for (int i = 0; i != static_cast<int>(NodeType::eLast); i++)
    //    {
    //        NodeType nodeType = static_cast<NodeType>(i);

    //        auto props = getPropertiesForType(nodeType);

    //        mShaders[nodeType] = createShaderFromFile(props.shaderPath);
    //    }
}

bool VulkanRenderer::createComputeRenderTarget(uint32_t width, uint32_t height)
{
    mComputeRenderTarget = std::unique_ptr<CsImage>(new CsImage(
        mWindow, &mDevice, &mPhysicalDevice, width, height, false, "Compute Render Target"));

    emit mWindow->renderTargetHasBeenCreated(width, height);

    mCurrentRenderSize = QSize(width, height);

    return true;
}

bool VulkanRenderer::createImageFromFile(const QString& path, const int colorSpace)
{
    mCpuImage = std::unique_ptr<ImageBuf>(new ImageBuf(path.toStdString()));
    bool ok   = mCpuImage->read(0, 0, 0, 4, true, OIIO::TypeDesc::FLOAT);
    if (!ok)
    {
        CS_LOG_WARNING("There was a problem reading the image from disk.");
        CS_LOG_WARNING(QString::fromStdString(mCpuImage->geterror()));
    }
    // Add alpha channel if it doesn't exist
    if (mCpuImage->nchannels() == 3)
    {
        int channelorder[]         = {0, 1, 2, -1};
        float channelvalues[]      = {0 /*ignore*/, 0 /*ignore*/, 0 /*ignore*/, 1.0};
        std::string channelnames[] = {"R", "G", "B", "A"};

        *mCpuImage =
            OIIO::ImageBufAlgo::channels(*mCpuImage, 4, channelorder, channelvalues, channelnames);
    }

    transformColorSpace(colorSpaces.at(colorSpace), "linear", *mCpuImage);

    updateVertexData(mCpuImage->xend(), mCpuImage->yend());

    vk::FormatProperties props = mPhysicalDevice.getFormatProperties(globalImageFormat);
    const bool canSampleLinear =
        (bool)(props.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);
    const bool canSampleOptimal =
        (bool)(props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage);
    if (!canSampleLinear && !canSampleOptimal)
    {
        CS_LOG_WARNING("Neither linear nor optimal image sampling is supported for image");
        return false;
    }

    // The image that gets the data from the CPU
    mLoadImageStaging = std::unique_ptr<CsImage>(new CsImage(
        mWindow,
        &mDevice,
        &mPhysicalDevice,
        mCpuImage->xend(),
        mCpuImage->yend(),
        true,
        "Load Image Staging"));

    if (!writeLinearImage(
            static_cast<float*>(mCpuImage->localpixels()),
            QSize(mCpuImage->xend(), mCpuImage->yend()),
            mLoadImageStaging))
    {
        CS_LOG_WARNING("Failed to write linear image");
        return false;
    }
    return true;
}

void VulkanRenderer::transformColorSpace(const QString& from, const QString& to, ImageBuf& image)
{
    parallelApplyColorSpace(
        mOcioConfig,
        from,
        to,
        static_cast<float*>(image.localpixels()),
        image.xend(),
        image.yend());
}

void VulkanRenderer::createComputeDescriptors()
{
    // TODO: Clean this up.

    if (!mComputeDescriptorSetLayout)
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

        vk::DescriptorSetLayoutCreateInfo descSetLayoutCreateInfo({}, 4, &bindings.at(0));

        mComputeDescriptorSetLayout =
            mDevice.createDescriptorSetLayoutUnique(descSetLayoutCreateInfo).value;
    }

    mGraphicsDescriptorSet.reserve(2);

    // Descriptor sets
    for (int i = 0; i < mConcurrentFrameCount; ++i)
    {
        {
            vk::DescriptorSetAllocateInfo descSetAllocInfo(
                *mDescriptorPool, 1, &(*mGraphicsDescriptorSetLayout));

            mGraphicsDescriptorSet.push_back(
                std::move(mDevice.allocateDescriptorSetsUnique(descSetAllocInfo).value.front()));
        }
    }

    vk::DescriptorSetAllocateInfo descSetAllocInfoCompute(
        *mDescriptorPool, 1, &(*mComputeDescriptorSetLayout));

    mComputeDescriptorSet =
        std::move(mDevice.allocateDescriptorSetsUnique(descSetAllocInfoCompute).value.front());
}

void VulkanRenderer::updateGraphicsDescriptors(
    const CsImage* const outputImage,
    const CsImage* const upstreamImage)
{
    for (int i = 0; i < mConcurrentFrameCount; ++i)
    {
        std::vector<vk::WriteDescriptorSet> descWrite(3);
        descWrite.at(0).dstSet          = *mGraphicsDescriptorSet.at(i);
        descWrite.at(0).dstBinding      = 0;
        descWrite.at(0).descriptorCount = 1;
        descWrite.at(0).descriptorType  = vk::DescriptorType::eUniformBuffer;
        descWrite.at(0).pBufferInfo     = &mUniformBufferInfo[i];

        vk::DescriptorImageInfo descImageInfo(
            *mSampler, *outputImage->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal);

        descWrite.at(1).dstSet          = *mGraphicsDescriptorSet.at(i);
        descWrite.at(1).dstBinding      = 1;
        descWrite.at(1).descriptorCount = 1;
        descWrite.at(1).descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descWrite.at(1).pImageInfo      = &descImageInfo;

        vk::DescriptorImageInfo descImageInfoUpstream(
            *mSampler, *upstreamImage->getImageView(), vk::ImageLayout::eShaderReadOnlyOptimal);

        descWrite.at(2).dstSet          = *mGraphicsDescriptorSet.at(i);
        descWrite.at(2).dstBinding      = 2;
        descWrite.at(2).descriptorCount = 1;
        descWrite.at(2).descriptorType  = vk::DescriptorType::eCombinedImageSampler;
        descWrite.at(2).pImageInfo      = &descImageInfoUpstream;

        mDevice.updateDescriptorSets(descWrite, {});
    }
}

void VulkanRenderer::updateComputeDescriptors(
    const CsImage* const inputImageBack,
    const CsImage* const inputImageFront,
    const CsImage* const outputImage)
{
    auto result = mDevice.waitIdle();
    Q_UNUSED(result);

    vk::DescriptorImageInfo sourceInfoBack(
        *mSampler, *inputImageBack->getImageView(), vk::ImageLayout::eGeneral);

    vk::DescriptorImageInfo sourceInfoFront;
    sourceInfoFront.sampler = *mSampler;
    if (inputImageFront)
        sourceInfoFront.imageView = *inputImageFront->getImageView();
    else
        sourceInfoFront.imageView = *inputImageBack->getImageView();
    sourceInfoFront.imageLayout = vk::ImageLayout::eGeneral;

    vk::DescriptorImageInfo destinationInfo(
        {}, *outputImage->getImageView(), vk::ImageLayout::eGeneral);

    vk::DescriptorBufferInfo settingsBufferInfo(*mSettingsBuffer->getBuffer(), 0, VK_WHOLE_SIZE);

    std::vector<vk::WriteDescriptorSet> descWrite(4);

    descWrite.at(0).dstSet          = *mComputeDescriptorSet;
    descWrite.at(0).dstBinding      = 0;
    descWrite.at(0).descriptorCount = 1;
    descWrite.at(0).descriptorType  = vk::DescriptorType::eStorageImage;
    descWrite.at(0).pImageInfo      = &sourceInfoBack;

    descWrite.at(1).dstSet          = *mComputeDescriptorSet;
    descWrite.at(1).dstBinding      = 1;
    descWrite.at(1).descriptorCount = 1;
    descWrite.at(1).descriptorType  = vk::DescriptorType::eStorageImage;
    descWrite.at(1).pImageInfo      = &sourceInfoFront;

    descWrite.at(2).dstSet          = *mComputeDescriptorSet;
    descWrite.at(2).dstBinding      = 2;
    descWrite.at(2).descriptorCount = 1;
    descWrite.at(2).descriptorType  = vk::DescriptorType::eStorageImage;
    descWrite.at(2).pImageInfo      = &destinationInfo;

    descWrite.at(3).dstSet          = *mComputeDescriptorSet;
    descWrite.at(3).dstBinding      = 3;
    descWrite.at(3).descriptorCount = 1;
    descWrite.at(3).descriptorType  = vk::DescriptorType::eUniformBuffer;
    descWrite.at(3).pBufferInfo     = &settingsBufferInfo;

    mDevice.updateDescriptorSets(descWrite, {});
}

void VulkanRenderer::createComputePipelineLayout()
{
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo({}, 1, &(*mComputeDescriptorSetLayout));

    //Create the layout, store it to share between shaders
    mComputePipelineLayout = mDevice.createPipelineLayoutUnique(pipelineLayoutInfo).value;
}

void VulkanRenderer::createComputePipelines()
{
    //    for (int i = 0; i != static_cast<int>(NodeType::eLast); i++)
    //    {
    //        NodeType nodeType = static_cast<NodeType>(i);

    //        mPipelines[nodeType] = createComputePipeline(mShaders[nodeType].get());
    //    }
}

vk::UniquePipeline VulkanRenderer::createComputePipeline(const vk::ShaderModule& shaderModule)
{
    vk::PipelineShaderStageCreateInfo computeStage(
        {}, vk::ShaderStageFlagBits::eCompute, shaderModule, "main");

    vk::ComputePipelineCreateInfo pipelineInfo({}, computeStage, *mComputePipelineLayout);

    vk::UniquePipeline pl =
        mDevice.createComputePipelineUnique(*mPipelineCache, pipelineInfo).value;

    return pl;
}

void VulkanRenderer::createQueryPool()
{
    vk::QueryPoolCreateInfo queryPoolInfo({}, vk::QueryType::eTimestamp, 2);

    mQueryPool = mDevice.createQueryPoolUnique(queryPoolInfo).value;
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

    vk::SubresourceLayout layout = mDevice.getImageSubresourceLayout(*image->getImage(), subres);

    float* p;
    vk::Result err = mDevice.mapMemory(
        *image->getMemory(), layout.offset, layout.size, {}, reinterpret_cast<void**>(&p));
    if (err != vk::Result::eSuccess)
    {
        CS_LOG_WARNING("Failed to map memory for linear image.");
        return false;
    }

    int pad = (layout.rowPitch - imgSize.width() * 16) / 4;

    // TODO: Parallelize this
    float* pixels = imgStart;
    int lineWidth = imgSize.width() * 16; // 4 channels * 4 bytes
    int w         = imgSize.width();
    int h         = imgSize.height();
    for (int y = 0; y < h; ++y)
    {
        memcpy(p, pixels, lineWidth);
        pixels += w * 4;
        p += w * 4 + pad;
    }

    mDevice.unmapMemory(*image->getMemory());

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
    // Projection matrix
    mProjection =
        mWindow->clipCorrectionMatrix(); // adjust for Vulkan-OpenGL clip space differences
    const QSize sz = mWindow->swapChainImageSize();
    mProjection.ortho(
        -sz.width() / mScaleXY,
        sz.width() / mScaleXY,
        -sz.height() / mScaleXY,
        sz.height() / mScaleXY,
        -1.0f,
        100.0f);
    mProjection.scale(500);
}

void VulkanRenderer::setDisplayMode(const DisplayMode mode)
{
    mDisplayMode = mode;
}

bool VulkanRenderer::saveImageToDisk(
    CsImage* const inputImage,
    const QString& path,
    const QMap<std::string, std::string>& attributes,
    const int colorSpace)
{
    bool success = true;

    auto mem = mComputeCommandBuffer->recordImageSave(inputImage);

    mComputeCommandBuffer->submitImageSave();

    auto result = mDevice.waitIdle();

    float* pInput;
    result = mDevice.mapMemory(*mem, 0, VK_WHOLE_SIZE, {}, reinterpret_cast<void**>(&pInput));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Failed to map memory.");

    int width     = inputImage->getWidth();
    int height    = inputImage->getHeight();
    int numValues = width * height * 4;

    float* output  = new float[numValues];
    float* pOutput = &output[0];

    parallelArrayCopy(pInput, pOutput, width, height);

    OIIO::ImageSpec spec(width, height, 4, OIIO::TypeDesc::FLOAT);
    QMap<std::string, std::string>::const_iterator it;
    for (it = attributes.begin(); it != attributes.end(); ++it)
    {
        spec.attribute(it.key(), it.value());
    }
    std::unique_ptr<ImageBuf> saveImage = std::unique_ptr<ImageBuf>(new ImageBuf(spec, output));

    transformColorSpace("linear", colorSpaces.at(colorSpace), *saveImage);

    success = saveImage->write(path.toStdString());

    if (!success)
    {
        CS_LOG_INFO("Problem saving image." + QString::fromStdString(saveImage->geterror()));
    }

    delete[] output;

    mDevice.unmapMemory(*mem);

    return success;
}

void VulkanRenderer::createRenderPass()
{
    vk::CommandBuffer cb = mWindow->currentCommandBuffer();

    const QSize sz = mWindow->swapChainImageSize();

    // Clear background
    vk::ClearDepthStencilValue clearDS = {1, 0};
    vk::ClearValue clearValues[2];
    clearValues[0].color        = clearColor;
    clearValues[1].depthStencil = clearDS;

    vk::RenderPassBeginInfo rpBeginInfo;
    rpBeginInfo.renderPass               = mWindow->defaultRenderPass();
    rpBeginInfo.framebuffer              = mWindow->currentFramebuffer();
    rpBeginInfo.renderArea.extent.width  = sz.width();
    rpBeginInfo.renderArea.extent.height = sz.height();
    rpBeginInfo.clearValueCount          = 2;
    rpBeginInfo.pClearValues             = clearValues;

    cb.beginRenderPass(rpBeginInfo, vk::SubpassContents::eInline);

    // TODO: Can we do this once?
    quint8* p;
    vk::Result err = mDevice.mapMemory(
        *mVertexBufferMemory,
        mUniformBufferInfo[mWindow->currentFrame()].offset,
        uniformDataSize,
        {},
        reinterpret_cast<void**>(&p));

    if (err != vk::Result::eSuccess)
    {
        CS_LOG_WARNING("Failed to map memory for vertex buffer.");
    }

    QMatrix4x4 m = mProjection;

    QMatrix4x4 rotation;
    rotation.setToIdentity();

    QMatrix4x4 translation;
    translation.setToIdentity();
    translation.translate(mPositionX, mPositionY, mPositionZ);

    QMatrix4x4 scale;
    scale.setToIdentity();
    scale.scale(mScaleXY, mScaleXY, mScaleXY);

    m = m * translation * scale;

    memcpy(p, m.constData(), 16 * sizeof(float));
    mDevice.unmapMemory(*mVertexBufferMemory);

    // Choose to either display RGB or Alpha
    vk::Pipeline* pl;
    if (mDisplayMode == DisplayMode::eAlpha)
        pl = &mGraphicsPipelineAlpha.get();
    else
        pl = &mGraphicsPipelineRGB.get();

    cb.pushConstants(
        *mGraphicsPipelineLayout,
        vk::ShaderStageFlagBits::eFragment,
        0,
        sizeof(mViewerPushConstants),
        mViewerPushConstants.data());
    cb.bindPipeline(vk::PipelineBindPoint::eGraphics, *pl);
    cb.bindDescriptorSets(
        vk::PipelineBindPoint::eGraphics,
        *mGraphicsPipelineLayout,
        0,
        *mGraphicsDescriptorSet.at(mWindow->currentFrame()),
        {});

    vk::DeviceSize vbOffset = 0;
    cb.bindVertexBuffers(0, 1, &mVertexBuffer.get(), &vbOffset);

    //negative viewport
    vk::Viewport viewport;
    viewport.x        = 0;
    viewport.y        = 0;
    viewport.width    = sz.width();
    viewport.height   = sz.height();
    viewport.minDepth = 0;
    viewport.maxDepth = 1;
    cb.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset.x = scissor.offset.y = 0;
    scissor.extent.width                = viewport.width;
    scissor.extent.height               = viewport.height;
    cb.setScissor(0, 1, &scissor);

    cb.draw(4, 1, 0, 0);

    cb.endRenderPass();
}

void VulkanRenderer::setViewerPushConstants(const QString& s)
{
    mViewerPushConstants = unpackPushConstants(s);
}

void VulkanRenderer::processReadNode(NodeBase* node)
{
    //    auto parts = node->getAllPropertyValues().split(",");
    //    int index = parts[parts.size() - 2].toInt();
    //    if ( index < 0 )
    //        index = 0;
    //    QString path = parts[index];
    //    int colorSpace = parts.last().toInt();

    //    QFileInfo checkFile(path);

    //    if(path != "" && checkFile.exists() && checkFile.isFile())
    //    {
    //        mImagePath = path;

    //        // Create texture
    //        if (!createImageFromFile(mImagePath, colorSpace))
    //            CS_LOG_WARNING("Failed to create texture");

    //        mTmpCacheImage = std::unique_ptr<CsImage>(
    //                    new CsImage(mWindow,
    //                                &mDevice,
    //                                &mPhysicalDevice,
    //                                mCpuImage->xend(),
    //                                mCpuImage->yend(),
    //                                false,
    //                                "Tmp Cache Image"));

    //        // Create render target
    //        if (!createComputeRenderTarget(mCpuImage->xend(), mCpuImage->yend()))
    //            CS_LOG_WARNING("Failed to create compute render target.");

    //        updateComputeDescriptors(mTmpCacheImage.get(), nullptr, mComputeRenderTarget.get());

    //        mComputeCommandBuffer->recordImageLoad(
    //                    mLoadImageStaging.get(),
    //                    mTmpCacheImage.get(),
    //                    mComputeRenderTarget.get(),
    //                    &mPipelines[NodeType::eRead].get());

    //        mComputeCommandBuffer->submitImageLoad();

    //        node->setCachedImage(std::move(mComputeRenderTarget));

    //        // Delete the staging image
    //        auto result = mDevice.waitIdle();
    //        Q_UNUSED(result);

    //        mLoadImageStaging = nullptr;
    //    }
    //    else
    //    {
    //        node->flushCache();
    //    }
}

void VulkanRenderer::processNode(
    NodeBase* node,
    CsImage* inputImageBack,
    CsImage* inputImageFront,
    const QSize targetSize)
{
    //    auto result = mDevice.waitIdle();

    //    fillSettingsBuffer(node);

    //    if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
    //        CS_LOG_WARNING("Failed to create compute render target.");

    //    // Tells the shader if we have a mask on the front input
    //    mSettingsBuffer->appendValue(0.0);
    //    if (inputImageFront)
    //    {
    //        mSettingsBuffer->incrementLastValue();
    //    }

    //    // TODO: This is a workaround for generative nodes without input
    //    // but needs to be fixed
    //    if (!inputImageBack)
    //    {
    //        mTmpCacheImage = std::unique_ptr<CsImage>(
    //                    new CsImage(mWindow,
    //                                &mDevice,
    //                                &mPhysicalDevice,
    //                                targetSize.width(),
    //                                targetSize.height(),
    //                                false,
    //                                "Tmp Cache Image"));
    //        inputImageBack = mTmpCacheImage.get();
    //    }

    //    auto pipeline = mPipelines[node->getType()].get();

    //    auto nodeType = node->getType();

    //    if (nodeType == NodeType::eShader || nodeType == NodeType::eIsf)
    //    {
    //        if (node->getShaderCode().size() != 0)
    //        {
    //            mShaderUser = createShaderFromCode(node->getShaderCode());

    //            mComputePipelineUser = createComputePipeline(mShaderUser.get());

    //            pipeline = mComputePipelineUser.get();
    //        }
    //        else
    //        {
    //            pipeline = mComputePipelineNoop.get();
    //        }
    //    }

    //    int numShaderPasses = getPropertiesForType(node->getType()).numShaderPasses;
    //    int currentShaderPass = 1;

    //    if (numShaderPasses == 1)
    //    {
    //        updateComputeDescriptors(inputImageBack, inputImageFront, mComputeRenderTarget.get());

    //        mComputeCommandBuffer->recordGeneric(
    //                    inputImageBack,
    //                    inputImageFront,
    //                    mComputeRenderTarget.get(),
    //                    pipeline,
    //                    numShaderPasses,
    //                    currentShaderPass);

    //        mComputeCommandBuffer->submitGeneric();

    //        mWindow->requestUpdate();

    //        result = mDevice.waitIdle();

    //        node->setCachedImage(std::move(mComputeRenderTarget));
    //    }
    //    else
    //    {
    //        for (int i = 1; i <= numShaderPasses; ++i)
    //        {
    //            // TODO: Shorten this
    //            if (currentShaderPass == 1)
    //            {
    //                // First pass of multipass shader
    //                mSettingsBuffer->appendValue(0.0);

    //                updateComputeDescriptors(inputImageBack, inputImageFront, mComputeRenderTarget.get());

    //                mComputeCommandBuffer->recordGeneric(
    //                            inputImageBack,
    //                            inputImageFront,
    //                            mComputeRenderTarget.get(),
    //                            pipeline,
    //                            numShaderPasses,
    //                            currentShaderPass);

    //                mComputeCommandBuffer->submitGeneric();
    //            }
    //            else if (currentShaderPass <= numShaderPasses)
    //            {
    //                // Subsequent passes
    //                mSettingsBuffer->incrementLastValue();

    //                if (!createComputeRenderTarget(targetSize.width(), targetSize.height()))
    //                    CS_LOG_WARNING("Failed to create compute render target.");

    //                updateComputeDescriptors(node->getCachedImage(), inputImageFront, mComputeRenderTarget.get());

    //                mComputeCommandBuffer->recordGeneric(
    //                            node->getCachedImage(),
    //                            inputImageFront,
    //                            mComputeRenderTarget.get(),
    //                            pipeline,
    //                            numShaderPasses,
    //                            currentShaderPass);

    //                mComputeCommandBuffer->submitGeneric();
    //            }
    //            currentShaderPass++;

    //            result = mDevice.waitIdle();

    //            node->setCachedImage(std::move(mComputeRenderTarget));
    //        }

    //        mWindow->requestUpdate();
    //    }
}

void VulkanRenderer::displayNode(const NodeBase* node)
{
    //    if(CsImage* image = node->getCachedImage())
    //    {
    //        // Execute a NoOp shader on the node
    //        mClearScreen = false;

    //        updateVertexData(image->getWidth(), image->getHeight());
    //        createVertexBuffer();

    //        if (!createComputeRenderTarget(image->getWidth(), image->getHeight()))
    //            CS_LOG_WARNING("Failed to create compute render target.");

    //        CsImage* upstreamImage = nullptr;
    //        if (node->getUpstreamNodeBack())
    //            upstreamImage = node->getUpstreamNodeBack()->getCachedImage();
    //        if (!upstreamImage)
    //            upstreamImage = image;

    //        updateGraphicsDescriptors(image, upstreamImage);
    //        updateComputeDescriptors(image, nullptr, mComputeRenderTarget.get());

    //        mComputeCommandBuffer->recordGeneric(
    //                    image,
    //                    nullptr,
    //                    mComputeRenderTarget.get(),
    //                    *mComputePipelineNoop,
    //                    1,
    //                    1);

    //        mComputeCommandBuffer->submitGeneric();

    //        mWindow->requestUpdate();
    //    }
    //    else
    //    {
    //        CS_LOG_INFO("Clearing screen");
    //        doClearScreen();
    //    }
}

void VulkanRenderer::doClearScreen()
{
    mClearScreen = true;

    mWindow->requestUpdate();
}

void VulkanRenderer::startNextFrame()
{
    if (mClearScreen)
    {
        const QSize sz = mWindow->swapChainImageSize();

        // Clear background
        vk::ClearDepthStencilValue clearDS = {1, 0};
        vk::ClearValue clearValues[2];
        clearValues[0].color        = clearColor;
        clearValues[1].depthStencil = clearDS;

        vk::CommandBuffer cmdBuf = mWindow->currentCommandBuffer();

        vk::RenderPassBeginInfo rpBeginInfo;
        rpBeginInfo.renderPass               = mWindow->defaultRenderPass();
        rpBeginInfo.framebuffer              = mWindow->currentFramebuffer();
        rpBeginInfo.renderArea.extent.width  = sz.width();
        rpBeginInfo.renderArea.extent.height = sz.height();
        rpBeginInfo.clearValueCount          = 2;
        rpBeginInfo.pClearValues             = clearValues;

        cmdBuf.beginRenderPass(&rpBeginInfo, vk::SubpassContents::eInline);

        cmdBuf.endRenderPass();
    }
    else
    {
        createRenderPass();
    }

    mWindow->frameReady();
}

void VulkanRenderer::logicalDeviceLost()
{
    emit mWindow->deviceLost();
}

void VulkanRenderer::translate(float dx, float dy)
{
    const QSize sz = mWindow->size();

    mPositionX += 6.0 * dx / sz.width();
    mPositionY += 2.0 * -dy / sz.height();

    mWindow->requestUpdate();
}

void VulkanRenderer::scale(float s)
{
    mScaleXY = s;
    mWindow->requestUpdate();
    emit mWindow->requestZoomTextUpdate(s);
}

void VulkanRenderer::releaseSwapChainResources()
{
    CS_LOG_INFO("Releasing swapchain resources.");
}

void VulkanRenderer::releaseResources()
{
    CS_LOG_INFO("Releasing resources.");
}

void VulkanRenderer::shutdown()
{
    auto result = mDevice.waitIdle();

    mLoadImageStaging    = nullptr;
    mTmpCacheImage       = nullptr;
    mComputeRenderTarget = nullptr;
    mSettingsBuffer      = nullptr;
    //    for(auto& pl : mPipelines)
    //        mDevice.destroy(*pl.second);
    mDevice.destroy(*mComputePipelineNoop);
    mDevice.destroy(*mComputePipelineUser);
    mDevice.destroy(*mGraphicsPipelineRGB);
    mDevice.destroy(*mGraphicsPipelineAlpha);
    mDevice.destroy(*mPipelineCache);
    mDevice.destroy(*mDescriptorPool);
    //    for(auto& sh : mShaders)
    //        mDevice.destroy(*sh.second);
    mDevice.destroy(*mShaderUser);
    mDevice.destroy(*mGraphicsPipelineLayout);
    mDevice.destroy(*mComputePipelineLayout);
    mDevice.destroy(*mGraphicsDescriptorSetLayout);
    mDevice.destroy(*mComputeDescriptorSetLayout);
    mComputeCommandBuffer = nullptr;
    mDevice.destroy(*mSampler);
    mDevice.free(*mVertexBufferMemory);
    mDevice.destroy(*mVertexBuffer);

    result = mDevice.waitIdle();
}

VulkanRenderer::~VulkanRenderer() {}

} // end namespace Cascade::Renderer
