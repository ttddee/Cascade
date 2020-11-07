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

#include "csimage.h"

CsImage::CsImage(
        VulkanWindow* win,
        const VkDevice* d,
        QVulkanDeviceFunctions* df,
        const int w,
        const int h)
        : devFuncs(df),
          device(d),
          width(w),
          height(h)
{
    window = win;

    VkImageCreateInfo imageInfo = {};

    imageInfo.sType             = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType         = VK_IMAGE_TYPE_2D;
    imageInfo.format            = VK_FORMAT_R32G32B32A32_SFLOAT;
    imageInfo.extent.width      = width;
    imageInfo.extent.height     = height;
    imageInfo.extent.depth      = 1;
    imageInfo.mipLevels         = 1;
    imageInfo.arrayLayers       = 1;
    imageInfo.samples           = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling            = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage             = VK_IMAGE_USAGE_SAMPLED_BIT |
                                  VK_IMAGE_USAGE_STORAGE_BIT |
                                  VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
                                  VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    imageInfo.sharingMode       = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout     = VK_IMAGE_LAYOUT_UNDEFINED;

    VkResult err = devFuncs->vkCreateImage(
                *device,
                &imageInfo,
                nullptr,
                &image);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create linear image for texture: %d", err);
    }

    //Get how much memory we need and how it should aligned
    VkMemoryRequirements memReq;
    devFuncs->vkGetImageMemoryRequirements(*device, image, &memReq);

    //The render target will be on the GPU
    uint32_t memIndex = window->deviceLocalMemoryIndex();

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

    err = devFuncs->vkAllocateMemory(
                *device,
                &allocInfo,
                nullptr,
                &memory);
    if (err != VK_SUCCESS) {
        qWarning("Failed to allocate memory for linear image: %d", err);
    }

    //Associate the image with this chunk of memory
    err = devFuncs->vkBindImageMemory(
                *device,
                image,
                memory,
                0);
    if (err != VK_SUCCESS) {
        qWarning("Failed to bind linear image memory: %d", err);
    }

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R32G32B32A32_SFLOAT;
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.levelCount = viewInfo.subresourceRange.layerCount = 1;

    err = devFuncs->vkCreateImageView(
                *device,
                &viewInfo,
                nullptr,
                &view);
    if (err != VK_SUCCESS) {
        qWarning("Failed to create image view for texture: %d", err);
    }
}

VkImage& CsImage::getImage()
{
    return image;
}

VkImageView& CsImage::getImageView()
{
    return view;
}

VkDeviceMemory& CsImage::getMemory()
{
    return memory;
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
    qDebug("Destroying image.");

    if (view) {
        devFuncs->vkDestroyImageView(*device, view, nullptr);
        view = VK_NULL_HANDLE;
    }
    if (image) {
        devFuncs->vkDestroyImage(*device, image, nullptr);
        image = VK_NULL_HANDLE;
    }
    if (memory) {
        devFuncs->vkFreeMemory(*device, memory, nullptr);
        memory = VK_NULL_HANDLE;
    }
}

CsImage::~CsImage()
{
    destroy();
}
