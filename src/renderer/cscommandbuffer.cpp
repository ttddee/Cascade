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

#include "cscommandbuffer.h"

#include "../log.h"
#include "renderconfig.h"

namespace Cascade::Renderer {

CsCommandBuffer::CsCommandBuffer(
        const vk::Device* d,
        const vk::PhysicalDevice* pd,
        vk::PipelineLayout* pipelineLayout,
        vk::DescriptorSet* descriptorSet) :
    device(d),
    physicalDevice(pd),
    mComputePipelineLayout(pipelineLayout),
    mComputeDescriptorSet(descriptorSet)
{
    createComputeQueue();
    createComputeCommandPool();
    createComputeCommandBuffers();

    CS_LOG_INFO("Created compute command buffer.");
}

void CsCommandBuffer::createComputeQueue()
{
    auto queueFamilyProperties = physicalDevice->getQueueFamilyProperties();

    for (unsigned int i = 0; i < queueFamilyProperties.size(); ++i)
    {
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)
        {
            computeFamilyIndex = i;
            break;
        }
    }

    // Get a compute queue from the device
    mComputeQueue = device->getQueue(computeFamilyIndex, 0);
}

void CsCommandBuffer::createComputeCommandPool()
{
    // Separate command pool as queue family for compute may be different than graphics
    vk::CommandPoolCreateInfo cmdPoolInfo(
                { vk::CommandPoolCreateFlagBits::eResetCommandBuffer },
                computeFamilyIndex);

    mComputeCommandPool = device->createCommandPoolUnique(cmdPoolInfo).value;
}

void CsCommandBuffer::createComputeCommandBuffers()
{
    // Create the command buffer for loading an image from disk
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo(
                *mComputeCommandPool,
                vk::CommandBufferLevel::ePrimary,
                3);

    std::vector<vk::UniqueCommandBuffer> buffers = device->allocateCommandBuffersUnique(
                commandBufferAllocateInfo).value;

    mCommandBufferImageLoad = vk::UniqueCommandBuffer(std::move(buffers.at(0)));
    mCommandBufferGeneric = vk::UniqueCommandBuffer(std::move(buffers.at(1)));
    mCommandBufferImageSave = vk::UniqueCommandBuffer(std::move(buffers.at(2)));

    // Fence for compute CB sync
    vk::FenceCreateInfo fenceCreateInfo(
                vk::FenceCreateFlagBits::eSignaled);

    mFence = device->createFenceUnique(fenceCreateInfo).value;
}

void CsCommandBuffer::recordGeneric(
        CsImage *const inputImageBack,
        CsImage *const inputImageFront,
        CsImage *const outputImage,
        vk::Pipeline &pl,
        int numShaderPasses,
        int currentShaderPass)
{
    auto result = mComputeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    result = mCommandBufferGeneric->begin(cmdBufferBeginInfo);

    // Layout transitions before compute stage
    inputImageBack->transitionLayoutTo(
                mCommandBufferGeneric,
                vk::ImageLayout::eGeneral);

    outputImage->transitionLayoutTo(
                mCommandBufferGeneric,
                vk::ImageLayout::eGeneral);

    if (inputImageFront)
    {
        inputImageFront->transitionLayoutTo(
                    mCommandBufferGeneric,
                    vk::ImageLayout::eGeneral);
    }

    mCommandBufferGeneric->bindPipeline(
                vk::PipelineBindPoint::eCompute,
                pl);
    mCommandBufferGeneric->bindDescriptorSets(
                vk::PipelineBindPoint::eCompute,
                *mComputePipelineLayout,
                0,
                *mComputeDescriptorSet,
                {});
    mCommandBufferGeneric->dispatch(
                outputImage->getWidth() / 16 + 1,
                outputImage->getHeight() / 16 + 1,
                1);

    // Layout transitions after compute stage
    inputImageBack->transitionLayoutTo(
                mCommandBufferGeneric,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    auto layout = vk::ImageLayout::eGeneral;
    if (currentShaderPass == numShaderPasses)
        layout = vk::ImageLayout::eShaderReadOnlyOptimal;

    outputImage->transitionLayoutTo(
                mCommandBufferGeneric,
                layout);

    if (inputImageFront)
    {
        inputImageFront->transitionLayoutTo(
                    mCommandBufferGeneric,
                    vk::ImageLayout::eShaderReadOnlyOptimal);
    }

    result = mCommandBufferGeneric->end();
    Q_UNUSED(result);
}

void CsCommandBuffer::recordImageLoad(
        CsImage* const loadImage,
        CsImage* const tmpImage,
        CsImage* const renderTarget,
        vk::Pipeline* const readNodePipeline)
{
     [[maybe_unused]] auto result = mComputeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    result = mCommandBufferImageLoad->begin(cmdBufferBeginInfo);

    loadImage->transitionLayoutTo(
                mCommandBufferImageLoad,
                vk::ImageLayout::eTransferSrcOptimal);

    tmpImage->transitionLayoutTo(
                mCommandBufferImageLoad,
                vk::ImageLayout::eTransferDstOptimal);

    vk::ImageCopy copyInfo;
    copyInfo.srcSubresource.aspectMask  = vk::ImageAspectFlagBits::eColor;
    copyInfo.srcSubresource.layerCount  = 1;
    copyInfo.dstSubresource.aspectMask  = vk::ImageAspectFlagBits::eColor;
    copyInfo.dstSubresource.layerCount  = 1;
    copyInfo.extent.width               = loadImage->getWidth();
    copyInfo.extent.height              = loadImage->getHeight();
    copyInfo.extent.depth               = 1;

    mCommandBufferImageLoad->copyImage(
                *loadImage->getImage(),
                vk::ImageLayout::eTransferSrcOptimal,
                *tmpImage->getImage(),
                vk::ImageLayout::eTransferDstOptimal,
                1,
                &copyInfo);

    tmpImage->transitionLayoutTo(
                mCommandBufferImageLoad,
                vk::ImageLayout::eGeneral);

    renderTarget->transitionLayoutTo(
                mCommandBufferImageLoad,
                vk::ImageLayout::eGeneral);

    mCommandBufferImageLoad->bindPipeline(
                vk::PipelineBindPoint::eCompute,
                *readNodePipeline);
    mCommandBufferImageLoad->bindDescriptorSets(
                vk::PipelineBindPoint::eCompute,
                *mComputePipelineLayout,
                0,
                *mComputeDescriptorSet,
                {});
    mCommandBufferImageLoad->dispatch(
                loadImage->getWidth() / 16 + 1,
                loadImage->getHeight() / 16 + 1,
                1);

    renderTarget->transitionLayoutTo(
                mCommandBufferImageLoad,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    result = mCommandBufferImageLoad->end();
}

vk::DeviceMemory* CsCommandBuffer::recordImageSave(
        CsImage *const inputImage)
{
    CS_LOG_INFO("Copying image GPU-->CPU.");

    // This is for outputting an image to the CPU
    auto result = mComputeQueue.waitIdle();

    vk::CommandBufferBeginInfo cmdBufferBeginInfo;

    result = mCommandBufferImageSave->begin(cmdBufferBeginInfo);

    auto outputImageSize = QSize(inputImage->getWidth(), inputImage->getHeight());

    vk::DeviceSize bufferSize = outputImageSize.width() * outputImageSize.height() * 16; // 4 channels * 4 bytes

    createBuffer(mOutputStagingBuffer, mOutputStagingBufferMemory, bufferSize);

    inputImage->transitionLayoutTo(
                mCommandBufferImageSave,
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
    mCommandBufferImageSave->copyImageToBuffer(
                *inputImage->getImage(),
                vk::ImageLayout::eTransferSrcOptimal,
                *mOutputStagingBuffer,
                copyInfo);

    inputImage->transitionLayoutTo(
                mCommandBufferImageSave,
                vk::ImageLayout::eShaderReadOnlyOptimal);

    result = mCommandBufferImageSave->end();
    Q_UNUSED(result);

    return &mOutputStagingBufferMemory.get();
}

void CsCommandBuffer::submitGeneric()
{
    // Submit compute commands
    // Use a fence to ensure that compute command buffer has finished executing before using it again
    vk::Result result = device->waitForFences(1, &(*mFence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*mFence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    // Do the copy on the compute queue
    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;

    computeSubmitInfo.pCommandBuffers = &mCommandBufferGeneric.get();

    result = mComputeQueue.submit(
                1,
                &computeSubmitInfo,
                *mFence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

void CsCommandBuffer::submitImageLoad()
{
    vk::Result result = device->waitForFences(1, &(*mFence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*mFence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    // Do the copy on the compute queue
    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;

    computeSubmitInfo.pCommandBuffers = &mCommandBufferImageLoad.get();

    result = mComputeQueue.submit(
                1,
                &computeSubmitInfo,
                *mFence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

void CsCommandBuffer::submitImageSave()
{
    vk::Result result = device->waitForFences(1, &(*mFence), true, UINT64_MAX);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem waiting for fence.");
    result = device->resetFences(1, &(*mFence));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Could not reset fence.");

    vk::SubmitInfo computeSubmitInfo;
    computeSubmitInfo.commandBufferCount = 1;
    computeSubmitInfo.pCommandBuffers = &mCommandBufferImageSave.get();

    result = mComputeQueue.submit(
                1,
                &computeSubmitInfo,
                *mFence);
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Problem submitting compute queue.");
}

vk::Queue* CsCommandBuffer::getQueue()
{
    return &mComputeQueue;
}

vk::CommandBuffer* CsCommandBuffer::getGeneric()
{
    auto result = mComputeQueue.waitIdle();
    Q_UNUSED(result);

    return &(*mCommandBufferGeneric);
}

vk::CommandBuffer* CsCommandBuffer::getImageLoad()
{
    auto result = mComputeQueue.waitIdle();
    Q_UNUSED(result);

    return &(*mCommandBufferImageLoad);
}

vk::CommandBuffer* CsCommandBuffer::getImageSave()
{
    return &(*mCommandBufferImageSave);
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

    buffer = device->createBufferUnique(bufferInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
                    vk::ObjectType::eBuffer,
                    NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkBuffer, *buffer),
                    "Output Staging Buffer");
        auto result = device->setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
        Q_UNUSED(result);
    }
#endif

    vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*buffer);

    uint32_t memoryType = findMemoryType(
                memRequirements.memoryTypeBits,
                vk::MemoryPropertyFlags(
                    vk::MemoryPropertyFlagBits::eHostVisible |
                    vk::MemoryPropertyFlagBits::eHostCoherent));

    vk::MemoryAllocateInfo allocInfo(memRequirements.size,
                                     memoryType);

    bufferMemory = device->allocateMemoryUnique(allocInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
                    vk::ObjectType::eDeviceMemory,
                    NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkDeviceMemory, *bufferMemory),
                    "Output Staging Buffer Memory");
        auto result = device->setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
        Q_UNUSED(result);
    }
#endif

    auto result = device->bindBufferMemory(*buffer, *bufferMemory, 0);
    Q_UNUSED(result);
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
}

} // namespace Cascade::Renderer
