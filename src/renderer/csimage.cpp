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

CsImage::CsImage(
        VulkanWindow* win,
        const vk::Device* d,
        const vk::PhysicalDevice* pd,
        const int w,
        const int h)
        : device(d),
          physicalDevice(pd),
          width(w),
          height(h)
{
    window = win;

    vk::ImageCreateInfo imageInfo({},
                                  vk::ImageType::e2D,
                                  vk::Format::eR32G32B32A32Sfloat,
                                  width,
                                  height,
                                  1,
                                  vk::SampleCountFlagBits::e1,
                                  vk::ImageTiling::eOptimal,
                                  vk::ImageUsageFlagBits::eSampled |
                                  vk::ImageUsageFlagBits::eStorage |
                                  vk::ImageUsageFlagBits::eTransferSrc |
                                  vk::ImageUsageFlagBits::eTransferDst,
                                  vk::SharingMode::eExclusive,
                                  {},
                                  {},
                                  vk::ImageLayout::eUndefined);

    try
    {
        image = device->createImageUnique(imageInfo);
    }
    catch (std::exception const &e)
    {
        CS_LOG_WARNING("Could not create unique image for CsImage.");
    }

    //Get how much memory we need and how it should aligned
    vk::MemoryRequirements memReq = device->getImageMemoryRequirements(*image);

    //The render target will be on the GPU
    uint32_t memIndex = window->deviceLocalMemoryIndex();

    if (!(memReq.memoryTypeBits & (1 << memIndex)))
    {
        vk::PhysicalDeviceMemoryProperties physDevMemProps = physicalDevice->getMemoryProperties();
        for (uint32_t i = 0; i < physDevMemProps.memoryTypeCount; ++i)
        {
            if (!(memReq.memoryTypeBits & (1 << i)))
                continue;
            memIndex = i;
        }
    }

    vk::MemoryAllocateInfo allocInfo(memReq.size,
                                     memIndex);
    try
    {
        memory = device->allocateMemoryUnique(allocInfo);
    }
    catch (std::exception const &e)
    {
        CS_LOG_WARNING("Could not allocate memory for CsImage.");
    }

    //Associate the image with this chunk of memory
    try
    {
        device->bindImageMemory(*image, *memory, 0);
    }
    catch (std::exception const &e)
    {
        CS_LOG_WARNING("Could not bind memory for CsImage.");
    }


    vk::ImageViewCreateInfo viewInfo(
                {},
                *image,
                vk::ImageViewType::e2D,
                vk::Format::eR32G32B32A32Sfloat,
                vk::ComponentMapping(vk::ComponentSwizzle::eR,
                                     vk::ComponentSwizzle::eG,
                                     vk::ComponentSwizzle::eB,
                                     vk::ComponentSwizzle::eA),
                vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor,
                                          {},
                                          1,
                                          0,
                                          1));

    try
    {
        view = device->createImageViewUnique(viewInfo);
    }
    catch (std::exception const &e)
    {
        CS_LOG_WARNING("Could not create image view for CsImage.");
    }
}

const vk::UniqueImage& CsImage::getImage() const
{
    return image;
}

const vk::UniqueImageView& CsImage::getImageView() const
{
    return view;
}

void CsImage::setImageView(vk::UniqueImageView v)
{
    view = std::move(v);
}

const vk::UniqueDeviceMemory& CsImage::getMemory() const
{
    return memory;
}

const vk::ImageLayout CsImage::getLayout() const
{
    return currentLayout;
}

void CsImage::setLayout(const vk::ImageLayout& layout)
{
    currentLayout = layout;
}

int CsImage::getWidth() const
{
    return width;
}

int CsImage::getHeight() const
{
    return height;
}

void CsImage::destroy()
{

}

CsImage::~CsImage()
{
    CS_LOG_INFO("Destroying image.");
}
