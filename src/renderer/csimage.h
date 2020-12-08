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

#ifndef CSIMAGE_H
#define CSIMAGE_H

#include <QVulkanDeviceFunctions>

#include <vulkan/vulkan.h>

#include "../vulkanwindow.h"

class CsImage
{
public:
    CsImage(
            VulkanWindow* win,
            const VkDevice* d,
            QVulkanDeviceFunctions* df,
            const int w = 100,
            const int h = 100);

    VkImage& getImage();
    VkImageView& getImageView();
    VkDeviceMemory& getMemory();

    VkImageLayout getLayout();
    void setLayout(VkImageLayout layout);

    int getWidth() const;
    int getHeight() const;

    void destroy();

    ~CsImage();

private:
    VkImage image           = VK_NULL_HANDLE;
    VkImageView view        = VK_NULL_HANDLE;
    VkDeviceMemory memory   = VK_NULL_HANDLE;

    VulkanWindow* window;
    QVulkanDeviceFunctions *devFuncs;
    const VkDevice* device;

    VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    const int width;
    const int height;
};

#endif // CSIMAGE_H
