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

#ifndef CSIMAGE_H
#define CSIMAGE_H

#include <QVulkanDeviceFunctions>

#include <vulkan/vulkan.h>

#include "vulkan/vulkan.hpp"

#include "../vulkanwindow.h"

namespace Cascade::Renderer
{

class CsImage
{
public:
    CsImage(
            VulkanWindow* win,
            const vk::Device* d,
            const vk::PhysicalDevice* pd,
            const int w = 100,
            const int h = 100,
            const bool isLinear = false);

    const vk::UniqueImage& getImage() const;
    const vk::UniqueImageView& getImageView() const;
    const vk::UniqueDeviceMemory& getMemory() const;

    const vk::ImageLayout getLayout() const;
    void transitionLayoutTo(vk::UniqueCommandBuffer& cb,
                            vk::ImageLayout layout);
    void setLayout(const vk::ImageLayout& layout);

    int getWidth() const;
    int getHeight() const;

    void destroy();

    ~CsImage();

private:
    vk::UniqueImage image;
    vk::UniqueImageView view;
    vk::UniqueDeviceMemory memory;

    VulkanWindow* window;
    const vk::Device* device;
    const vk::PhysicalDevice* physicalDevice;

    vk::ImageLayout currentLayout = vk::ImageLayout::eUndefined;

    const int width;
    const int height;
};

} // end namespace Cascade::Renderer

#endif // CSIMAGE_H
