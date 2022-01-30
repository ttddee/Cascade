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

#ifndef CSCOMMANDBUFFER_H
#define CSCOMMANDBUFFER_H

#include "csimage.h"

namespace Cascade::Renderer
{

class CsCommandBuffer
{
public:
    CsCommandBuffer(
            const vk::Device* d,
            const vk::PhysicalDevice* pd,
            vk::PipelineLayout* pipelineLayout,
            vk::DescriptorSet* descriptorSet);

    void recordGeneric(
            CsImage* const inputImageBack,
            CsImage* const inputImageFront,
            CsImage* const outputImage,
            vk::Pipeline& pl,
            int numShaderPasses,
            int currentShaderPass);
    void recordImageLoad(
            CsImage* const loadImage,
            CsImage* const tmpImage,
            CsImage* const renderTarget,
            vk::Pipeline* const readNodePipeline);
    vk::DeviceMemory* recordImageSave(
            CsImage* const inputImage);

    void submitGeneric();
    void submitImageLoad();
    void submitImageSave();

    ~CsCommandBuffer();

    vk::Queue* getQueue();
    vk::CommandBuffer* getGeneric();
    vk::CommandBuffer* getImageLoad();
    vk::CommandBuffer* getImageSave();

private:
    void createComputeQueue();
    void createComputeCommandPool();
    void createComputeCommandBuffers();

    void createBuffer(
            vk::UniqueBuffer& buffer,
            vk::UniqueDeviceMemory& bufferMemory,
            vk::DeviceSize& size);

    uint32_t findMemoryType(
            uint32_t typeFilter,
            vk::MemoryPropertyFlags properties);

    const vk::Device* device;
    const vk::PhysicalDevice* physicalDevice;
    int computeFamilyIndex;

    vk::UniqueCommandPool computeCommandPool;
    // Command buffer for all shaders except IO
    vk::UniqueCommandBuffer commandBufferGeneric;
    // Command buffer for loading images from disk
    vk::UniqueCommandBuffer commandBufferImageLoad;
    // Command buffer for writing images to disk
    vk::UniqueCommandBuffer commandBufferImageSave;

    vk::CommandBuffer* currentBuffer;

    vk::Queue computeQueue;
    vk::UniqueFence fence;

    vk::PipelineLayout* computePipelineLayout;
    vk::DescriptorSet* computeDescriptorSet;

    vk::UniqueBuffer outputStagingBuffer;
    vk::UniqueDeviceMemory outputStagingBufferMemory;
};

} // end namespace Cascade::Renderer

#endif // CSCOMMANDBUFFER_H
