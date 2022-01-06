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

#include "cssettingsbuffer.h"

#include <stdexcept>
#include <iostream>

#include <QString>
#include <QStringList>

#include "../log.h"

CsSettingsBuffer::CsSettingsBuffer(
        VkDevice* dev,
        VkPhysicalDevice* physicalDevice,
        QVulkanDeviceFunctions* df,
        QVulkanFunctions* f)
{
    device = dev;
    devFuncs = df;

    VkDeviceSize size = sizeof(float) * 128;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (devFuncs->vkCreateBuffer(*device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    devFuncs->vkGetBufferMemoryRequirements(*device, buffer, &memRequirements);

    VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkPhysicalDeviceMemoryProperties memProperties;
    f->vkGetPhysicalDeviceMemoryProperties(*physicalDevice, &memProperties);

    uint32_t memTypeIndex = 0;

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            memTypeIndex = i;
        }
    }

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memTypeIndex;

    if (devFuncs->vkAllocateMemory(*device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate buffer memory!");
    }

    devFuncs->vkBindBufferMemory(*device, buffer, memory, 0);

    VkResult err = devFuncs->vkMapMemory(
                    *device,
                    memory,
                    0,
                    VK_WHOLE_SIZE,
                    0,
                    reinterpret_cast<void **>(&pBufferStart));
    if (err != VK_SUCCESS)
        {
            CS_LOG_WARNING("Failed to map memory for staging buffer.");
        }
}

void CsSettingsBuffer::fillBuffer(const QString &s)
{
    auto parts = s.split(",");

    float* pBuffer = pBufferStart;

    bufferSize = 0;

    foreach(auto& item, parts)
    {
        *pBuffer = item.toFloat();
        pBuffer++;
        bufferSize++;
    }
}

void CsSettingsBuffer::appendValue(float f)
{
    float *pBuffer = pBufferStart;
    pBuffer += bufferSize;
    *pBuffer = f;
    bufferSize++;
}

void CsSettingsBuffer::incrementLastValue()
{
    float *pBuffer = pBufferStart;
    pBuffer += bufferSize - 1;
    *pBuffer = *pBuffer + 1.0;
}

VkBuffer* CsSettingsBuffer::getBuffer()
{
    return &buffer;
}

VkDeviceMemory* CsSettingsBuffer::getMemory()
{
    return &memory;
}

CsSettingsBuffer::~CsSettingsBuffer()
{
    devFuncs->vkUnmapMemory(*device, memory);

    if (buffer) {
        devFuncs->vkDestroyBuffer(*device, buffer, nullptr);
        buffer = VK_NULL_HANDLE;
    }

    if (memory) {
        devFuncs->vkFreeMemory(*device, memory, nullptr);
        memory = VK_NULL_HANDLE;
    }
}
