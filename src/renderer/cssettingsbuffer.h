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

#ifndef CSSETTINGSBUFFER_H
#define CSSETTINGSBUFFER_H

#include <QVulkanDeviceFunctions>

#include <vulkan/vulkan.h>

class CsSettingsBuffer
{
public:
    CsSettingsBuffer(
            VkDevice* dev,
            VkPhysicalDevice* physicalDevice,
            QVulkanDeviceFunctions* df,
            QVulkanFunctions* f);

    void fillBuffer(const QString& s);
    void appendValue(float f);
    void incrementLastValue();

    VkBuffer* getBuffer();
    VkDeviceMemory* getMemory();

    ~CsSettingsBuffer();

private:
    VkBuffer buffer;
    VkDeviceMemory memory;

    VkDevice* device;
    QVulkanDeviceFunctions* devFuncs;

    float* pBufferStart;
    int bufferSize;
};

#endif // CSSETTINGSBUFFER_H
