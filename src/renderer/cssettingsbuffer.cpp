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
        vk::Device* d,
        vk::PhysicalDevice* pd)
{
    device = d;
    physicalDevice = pd;

    vk::DeviceSize size = sizeof(float) * 128;

    vk::BufferCreateInfo bufferInfo(
                {},
                size,
                vk::BufferUsageFlags(
                    vk::BufferUsageFlagBits::eUniformBuffer |
                    vk::BufferUsageFlagBits::eTransferDst
                    ),
                vk::SharingMode::eExclusive);

    buffer = device->createBufferUnique(bufferInfo);

    vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*buffer);

    vk::MemoryPropertyFlags properties =
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent;

    vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice->getMemoryProperties();

    uint32_t memTypeIndex = 0;

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            memTypeIndex = i;
        }
    }

    vk::MemoryAllocateInfo allocInfo(
                memRequirements.size,
                memTypeIndex);

    memory = device->allocateMemoryUnique(allocInfo);

    device->bindBufferMemory(*buffer, *memory, 0);

    device->mapMemory(
                *memory,
                0,
                VK_WHOLE_SIZE,
                {},
                reinterpret_cast<void **>(&pBufferStart));
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

vk::UniqueBuffer& CsSettingsBuffer::getBuffer()
{
    return buffer;
}

vk::UniqueDeviceMemory& CsSettingsBuffer::getMemory()
{
    return memory;
}

CsSettingsBuffer::~CsSettingsBuffer()
{
//    devFuncs->vkUnmapMemory(*device, memory);

//    if (buffer) {
//        devFuncs->vkDestroyBuffer(*device, buffer, nullptr);
//        buffer = VK_NULL_HANDLE;
//    }

//    if (memory) {
//        devFuncs->vkFreeMemory(*device, memory, nullptr);
//        memory = VK_NULL_HANDLE;
//    }
}
