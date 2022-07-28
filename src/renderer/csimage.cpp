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

#include "csimage.h"

#include "../log.h"
#include "renderconfig.h"
#include "../benchmark.h"

namespace Cascade::Renderer {

CsImage::CsImage(
        VulkanWindow* win,
        const vk::Device* d,
        const vk::PhysicalDevice* pd,
        const int w,
        const int h,
        const bool isLinear,
        const char* debugName)
        : mDevice(d),
          mPhysicalDevice(pd),
          mWidth(w),
          mHeight(h)
{
    mWindow = win;

    isLinear ? mCurrentLayout = vk::ImageLayout::eUndefined :
               mCurrentLayout = vk::ImageLayout::ePreinitialized;

    vk::ImageCreateInfo imageInfo(
                {},
                vk::ImageType::e2D,
                vk::Format::eR32G32B32A32Sfloat,
                vk::Extent3D(mWidth, mHeight, 1),
                1,
                1,
                vk::SampleCountFlagBits::e1,
                isLinear ? vk::ImageTiling::eLinear :
                           vk::ImageTiling::eOptimal,
                isLinear ? vk::ImageUsageFlagBits::eSampled |
                           vk::ImageUsageFlagBits::eTransferSrc :
                           vk::ImageUsageFlagBits::eSampled |
                           vk::ImageUsageFlagBits::eStorage |
                           vk::ImageUsageFlagBits::eTransferSrc |
                           vk::ImageUsageFlagBits::eTransferDst,
                vk::SharingMode::eExclusive,
                {},
                {},
                mCurrentLayout);
    mImage = mDevice->createImageUnique(imageInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
                    vk::ObjectType::eImage,
                    NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkImage, *mImage),
                    debugName);
         [[maybe_unused]] auto result = mDevice->setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
    }
#endif

    // Get how much memory we need and how it should aligned
    vk::MemoryRequirements memReq = mDevice->getImageMemoryRequirements(*mImage);

    // Make sure linear images get memory visible to the CPU
    uint32_t memIndex = 0;

    isLinear ? memIndex = mWindow->hostVisibleMemoryIndex() :
               memIndex = mWindow->deviceLocalMemoryIndex();

    if (!(memReq.memoryTypeBits & (1 << memIndex)))
    {
        vk::PhysicalDeviceMemoryProperties physDevMemProps = mPhysicalDevice->getMemoryProperties();
        for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i)
        {
            if (!(memReq.memoryTypeBits & (1 << i)))
                continue;
            memIndex = i;
        }
    }

    vk::MemoryAllocateInfo allocInfo(memReq.size, memIndex);
    mMemory = mDevice->allocateMemoryUnique(allocInfo).value;

#ifdef QT_DEBUG
    {
        vk::DebugUtilsObjectNameInfoEXT debugUtilsObjectNameInfo(
                    vk::ObjectType::eDeviceMemory,
                    NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(VkDeviceMemory, *mMemory),
                    debugName);
         [[maybe_unused]] auto result = mDevice->setDebugUtilsObjectNameEXT(debugUtilsObjectNameInfo);
    }
#endif

    //Associate the image with this chunk of memory
     [[maybe_unused]] auto result = mDevice->bindImageMemory(*mImage, *mMemory, 0);

    vk::ImageViewCreateInfo viewInfo(
                { },
                *mImage,
                vk::ImageViewType::e2D,
                vk::Format::eR32G32B32A32Sfloat,
                vk::ComponentMapping(vk::ComponentSwizzle::eR,
                                     vk::ComponentSwizzle::eG,
                                     vk::ComponentSwizzle::eB,
                                     vk::ComponentSwizzle::eA),
                vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor,
                                          0,
                                          1,
                                          0,
                                          1));

    mView = mDevice->createImageViewUnique(viewInfo).value;
}

const vk::UniqueImage& CsImage::getImage() const
{
    return mImage;
}

const vk::UniqueImageView& CsImage::getImageView() const
{
    return mView;
}

const vk::UniqueDeviceMemory& CsImage::getMemory() const
{
    return mMemory;
}

vk::ImageLayout CsImage::getLayout() const
{
    return mCurrentLayout;
}

void CsImage::transitionLayoutTo(vk::UniqueCommandBuffer &cb, vk::ImageLayout layout)
{
    // Note: The command buffer must be active
    vk::ImageMemoryBarrier barrier(
                vk::AccessFlagBits::eMemoryRead,
                vk::AccessFlagBits::eShaderRead |
                vk::AccessFlagBits::eShaderWrite,
                mCurrentLayout,
                layout,
                {},
                {},
                *mImage,
                vk::ImageSubresourceRange
                {
                    vk::ImageAspectFlagBits::eColor,
                    0,
                    1,
                    0,
                    1});

    cb->pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eComputeShader,
                {},
                {},
                {},
                barrier);

    setLayout(layout);
}

void CsImage::setLayout(const vk::ImageLayout& layout)
{
    mCurrentLayout = layout;
}

int CsImage::getWidth() const
{
    return mWidth;
}

int CsImage::getHeight() const
{
    return mHeight;
}

void CsImage::destroy()
{

}

CsImage::~CsImage()
{
    // Need to make sure this image is not used by any command buffer
    auto result = mDevice->waitIdle();
    Q_UNUSED(result);
}

} // end namespace Cascade::Renderer
