#include "cscommandbuffer.h"

#include "../log.h"

namespace Cascade::Renderer
{

CsCommandBuffer::CsCommandBuffer(
        const vk::Device* d,
        const vk::PhysicalDevice* pd,
        vk::PipelineLayout* pipelineLayout,
        vk::DescriptorSet* descriptorSet) :
    device(d),
    physicalDevice(pd),
    computePipelineLayout(pipelineLayout),
    computeDescriptorSet(descriptorSet)
{
    createComputeQueue();
    createComputeCommandPool();
    createComputeCommandBuffers();

    currentBuffer = &commandBufferGeneric.get();

    CS_LOG_INFO("Created compute command buffer.");
}

void CsCommandBuffer::createComputeQueue()
{
    auto queueFamilyProperties = physicalDevice->getQueueFamilyProperties();

    for (auto i = 0; i < queueFamilyProperties.size(); ++i)
    {
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)
        {
            computeFamilyIndex = i;
            break;
        }
    }

    // Get a compute queue from the device
    computeQueue = device->getQueue(computeFamilyIndex, 0);
}

void CsCommandBuffer::createComputeCommandPool()
{
    // Separate command pool as queue family for compute may be different than graphics
    vk::CommandPoolCreateInfo cmdPoolInfo(
                { vk::CommandPoolCreateFlagBits::eResetCommandBuffer },
                computeFamilyIndex);

    computeCommandPool = device->createCommandPoolUnique(cmdPoolInfo);
}

void CsCommandBuffer::createComputeCommandBuffers()
{
    // Create the command buffer for loading an image from disk
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo(
                *computeCommandPool,
                vk::CommandBufferLevel::ePrimary,
                3);

    std::vector<vk::UniqueCommandBuffer> buffers = device->allocateCommandBuffersUnique(
                commandBufferAllocateInfo);

    commandBufferImageLoad = vk::UniqueCommandBuffer(std::move(buffers.at(0)));
    commandBufferGeneric = vk::UniqueCommandBuffer(std::move(buffers.at(1)));
    commandBufferImageSave = vk::UniqueCommandBuffer(std::move(buffers.at(2)));

    // Fence for compute CB sync
    vk::FenceCreateInfo fenceCreateInfo(
                vk::FenceCreateFlagBits::eSignaled);

    fence = device->createFenceUnique(fenceCreateInfo);
}

void CsCommandBuffer::recordGeneric(
        CsImage *const inputImageBack,
        CsImage *const inputImageFront,
        CsImage *const outputImage,
        vk::Pipeline &pl,
        int numShaderPasses,
        int currentShaderPass)
{
    currentBuffer = &commandBufferGeneric.get();

    computeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    commandBufferGeneric->begin(cmdBufferBeginInfo);

    // Layout transitions before compute stage
    inputImageBack->transitionLayoutTo(
                commandBufferGeneric,
                vk::ImageLayout::eGeneral);

    outputImage->transitionLayoutTo(
                commandBufferGeneric,
                vk::ImageLayout::eGeneral);

    if (inputImageFront)
    {
        inputImageFront->transitionLayoutTo(
                    commandBufferGeneric,
                    vk::ImageLayout::eGeneral);
    }

    commandBufferGeneric->bindPipeline(
                vk::PipelineBindPoint::eCompute,
                pl);
    commandBufferGeneric->bindDescriptorSets(
                vk::PipelineBindPoint::eCompute,
                *computePipelineLayout,
                0,
                *computeDescriptorSet,
                {});
    commandBufferGeneric->dispatch(
                outputImage->getWidth() / 16 + 1,
                outputImage->getHeight() / 16 + 1,
                1);

    // Layout transitions after compute stage
    inputImageBack->transitionLayoutTo(
                commandBufferGeneric,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    auto layout = vk::ImageLayout::eGeneral;
    if (currentShaderPass == numShaderPasses)
        layout = vk::ImageLayout::eShaderReadOnlyOptimal;

    outputImage->transitionLayoutTo(
                commandBufferGeneric,
                layout);

    if (inputImageFront)
    {
        inputImageFront->transitionLayoutTo(
                    commandBufferGeneric,
                    vk::ImageLayout::eShaderReadOnlyOptimal);
    }

    commandBufferGeneric->end();
}

void CsCommandBuffer::recordImageLoad(
        CsImage* const loadImage,
        CsImage* const tmpImage,
        CsImage* const renderTarget,
        vk::Pipeline* const readNodePipeline)
{
    currentBuffer = &commandBufferImageLoad.get();

    computeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    commandBufferImageLoad->begin(cmdBufferBeginInfo);

    loadImage->transitionLayoutTo(
                commandBufferImageLoad,
                vk::ImageLayout::eTransferSrcOptimal);

    tmpImage->transitionLayoutTo(
                commandBufferImageLoad,
                vk::ImageLayout::eTransferDstOptimal);

    vk::ImageCopy copyInfo;
    copyInfo.srcSubresource.aspectMask  = vk::ImageAspectFlagBits::eColor;
    copyInfo.srcSubresource.layerCount  = 1;
    copyInfo.dstSubresource.aspectMask  = vk::ImageAspectFlagBits::eColor;
    copyInfo.dstSubresource.layerCount  = 1;
    copyInfo.extent.width               = loadImage->getWidth();
    copyInfo.extent.height              = loadImage->getHeight();
    copyInfo.extent.depth               = 1;

    commandBufferImageLoad->copyImage(
                *loadImage->getImage(),
                vk::ImageLayout::eTransferSrcOptimal,
                *tmpImage->getImage(),
                vk::ImageLayout::eTransferDstOptimal,
                1,
                &copyInfo);

    tmpImage->transitionLayoutTo(
                commandBufferImageLoad,
                vk::ImageLayout::eGeneral);

    renderTarget->transitionLayoutTo(
                commandBufferImageLoad,
                vk::ImageLayout::eGeneral);

    commandBufferImageLoad->bindPipeline(
                vk::PipelineBindPoint::eCompute,
                *readNodePipeline);
    commandBufferImageLoad->bindDescriptorSets(
                vk::PipelineBindPoint::eCompute,
                *computePipelineLayout,
                0,
                *computeDescriptorSet,
                {});
    commandBufferImageLoad->dispatch(
                loadImage->getWidth() / 16 + 1,
                loadImage->getHeight() / 16 + 1,
                1);

    renderTarget->transitionLayoutTo(
                commandBufferImageLoad,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    commandBufferImageLoad->end();
}

vk::DeviceMemory* CsCommandBuffer::recordImageSave(
        CsImage *const inputImage)
{
    CS_LOG_INFO("Copying image GPU-->CPU.");

    currentBuffer = &commandBufferImageSave.get();

    // This is for outputting an image to the CPU
    computeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    commandBufferImageSave->begin(cmdBufferBeginInfo);

    auto outputImageSize = QSize(inputImage->getWidth(), inputImage->getHeight());

    vk::DeviceSize bufferSize = outputImageSize.width() * outputImageSize.height() * 16; // 4 channels * 4 bytes

    createBuffer(outputStagingBuffer, outputStagingBufferMemory, bufferSize);

    inputImage->transitionLayoutTo(
                commandBufferImageSave,
                vk::ImageLayout::eTransferSrcOptimal);

    vk::ImageSubresourceLayers imageLayers(
                vk::ImageAspectFlagBits::eColor,
                {},
                0,
                1);

    vk::BufferImageCopy copyInfo(
                0,
                outputImageSize.width(),
                outputImageSize.height(),
                imageLayers,
                { 0, 0, 0 },
                {
                    (uint32_t)outputImageSize.width(),
                    (uint32_t)outputImageSize.height(),
                    1
                });
    commandBufferImageSave->copyImageToBuffer(
                *inputImage->getImage(),
                vk::ImageLayout::eTransferSrcOptimal,
                *outputStagingBuffer,
                copyInfo);

    inputImage->transitionLayoutTo(
                commandBufferImageSave,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    commandBufferImageSave->end();

    return &(*outputStagingBufferMemory);
}

void CsCommandBuffer::submitGeneric()
{
    // Submit compute commands
    // Use a fence to ensure that compute command buffer has finished executing before using it again
    vk::Result result = device->waitForFences(1, &(*fence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*fence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    // Do the copy on the compute queue
    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;

    computeSubmitInfo.pCommandBuffers = &commandBufferGeneric.get();

    result = computeQueue.submit(
                1,
                &computeSubmitInfo,
                *fence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

void CsCommandBuffer::submitImageLoad()
{
    vk::Result result = device->waitForFences(1, &(*fence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*fence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    // Do the copy on the compute queue
    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;

    computeSubmitInfo.pCommandBuffers = &commandBufferImageLoad.get();

    result = computeQueue.submit(
                1,
                &computeSubmitInfo,
                *fence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

void CsCommandBuffer::submitImageSave()
{
    vk::Result result = device->waitForFences(1, &(*fence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*fence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;
    computeSubmitInfo.pCommandBuffers = &commandBufferImageSave.get();

    result = computeQueue.submit(
                1,
                &computeSubmitInfo,
                *fence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

vk::Queue* CsCommandBuffer::getQueue()
{
    return &computeQueue;
}

vk::CommandBuffer* CsCommandBuffer::getGeneric()
{
    computeQueue.waitIdle();

    return &(*commandBufferGeneric);
}

vk::CommandBuffer* CsCommandBuffer::getImageLoad()
{
    computeQueue.waitIdle();

    return &(*commandBufferImageLoad);
}

vk::CommandBuffer* CsCommandBuffer::getImageSave()
{
    return &(*commandBufferImageSave);
}

void CsCommandBuffer::createBuffer(
        vk::UniqueBuffer& buffer,
        vk::UniqueDeviceMemory& bufferMemory,
        vk::DeviceSize& size)
{
    vk::BufferCreateInfo bufferInfo(
                {},
                size,
                vk::BufferUsageFlags(
                    vk::BufferUsageFlagBits::eTransferDst |
                    vk::BufferUsageFlagBits::eUniformBuffer),
                vk::SharingMode::eExclusive);

    buffer = device->createBufferUnique(bufferInfo);

    vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*buffer);

    uint32_t memoryType = findMemoryType(
                memRequirements.memoryTypeBits,
                vk::MemoryPropertyFlags(
                    vk::MemoryPropertyFlagBits::eHostVisible |
                    vk::MemoryPropertyFlagBits::eHostCoherent));

    vk::MemoryAllocateInfo allocInfo(memRequirements.size,
                                     memoryType);

    bufferMemory = device->allocateMemoryUnique(allocInfo);

    device->bindBufferMemory(*buffer, *bufferMemory, 0);
}

vk::CommandBuffer* CsCommandBuffer::getCurrent()
{
    return currentBuffer;
}

uint32_t CsCommandBuffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice->getMemoryProperties();

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type!");
}

CsCommandBuffer::~CsCommandBuffer()
{
    CS_LOG_INFO("Destroying command buffer.");
    if (outputStagingBufferMemory)
        device->freeMemory(*outputStagingBufferMemory);
    if (outputStagingBuffer)
        device->destroy(*outputStagingBuffer);
}

} // end namespace Cascade::Renderer
