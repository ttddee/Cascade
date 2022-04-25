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

#include <QString>
#include <QStringList>

#include "../log.h"

namespace Cascade::Renderer {

CsSettingsBuffer::CsSettingsBuffer(
        vk::Device* d,
        vk::PhysicalDevice* pd)
{
    mDevice = d;
    mPhysicalDevice = pd;

    vk::DeviceSize size = sizeof(float) * 128;

    vk::BufferCreateInfo bufferInfo(
                {},
                size,
                vk::BufferUsageFlags(
                    vk::BufferUsageFlagBits::eUniformBuffer |
                    vk::BufferUsageFlagBits::eTransferDst
                    ),
                vk::SharingMode::eExclusive);

    mBuffer = mDevice->createBufferUnique(bufferInfo).value;

    vk::MemoryRequirements memRequirements = mDevice->getBufferMemoryRequirements(*mBuffer);

    vk::MemoryPropertyFlags properties =
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent;

    vk::PhysicalDeviceMemoryProperties memProperties = mPhysicalDevice->getMemoryProperties();

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

   mMemory = mDevice->allocateMemoryUnique(allocInfo).value;

    auto result = mDevice->bindBufferMemory(*mBuffer, *mMemory, 0);

    result = mDevice->mapMemory(
                *mMemory,
                0,
                VK_WHOLE_SIZE,
                {},
                reinterpret_cast<void **>(&mBufferStart));
    if (result != vk::Result::eSuccess)
        CS_LOG_WARNING("Failed to map memory");
}

void CsSettingsBuffer::fillBuffer(const QString &s)
{
    auto parts = s.split(",");

    float* pBuffer = mBufferStart;

    mBufferSize = 0;

    foreach(auto& item, parts)
    {
        *pBuffer = item.toFloat();
        pBuffer++;
        mBufferSize++;
    }
}

void CsSettingsBuffer::appendValue(float f)
{
    float *pBuffer = mBufferStart;
    pBuffer += mBufferSize;
    *pBuffer = f;
    mBufferSize++;
}

void CsSettingsBuffer::incrementLastValue()
{
    float *pBuffer = mBufferStart;
    pBuffer += mBufferSize - 1;
    *pBuffer = *pBuffer + 1.0;
}

vk::UniqueBuffer& CsSettingsBuffer::getBuffer()
{
    return mBuffer;
}

vk::UniqueDeviceMemory& CsSettingsBuffer::getMemory()
{
    return mMemory;
}

CsSettingsBuffer::~CsSettingsBuffer()
{

}

} // end namespace Cascade::Renderer
