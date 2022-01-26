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

#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <array>

#include <QVulkanWindow>
#include <QImage>

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/color.h>
#include <OpenColorIO/OpenColorIO.h>

#include "vulkan/vulkan.hpp"

#include "../nodedefinitions.h"
#include "../nodebase.h"
#include "../windowmanager.h"
#include "cssettingsbuffer.h"
#include "csimage.h"

using namespace Cascade;
using namespace OIIO;
namespace OCIO = OCIO_NAMESPACE;

class VulkanWindow;

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    VulkanRenderer(VulkanWindow *w);

    // Initialize
    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void processReadNode(
            NodeBase* node);
    void processNode(
            NodeBase* node,
            CsImage* inputImageBack,
            CsImage* inputImageFront,
            const QSize targetSize);
    void displayNode(
            const NodeBase* node);
    void doClearScreen();
    void setDisplayMode(
            const DisplayMode mode);

    bool saveImageToDisk(
            CsImage* const inputImage,
            const QString& path,
            const int colorSpace);

    void setViewerPushConstants(const QString& s);

    void startNextFrame() override;

    QString getGpuName();

    void translate(float dx, float dy);
    void scale(float s);

    void cleanup();

    ~VulkanRenderer();

private:
    // Initialize
    VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pCallback);

    void createVertexBuffer();
    void createSampler();
    void createDescriptorPool();
    void createGraphicsDescriptors();
    void createGraphicsPipelineCache();
    void createGraphicsPipelineLayout();
    void createGraphicsPipeline(
            vk::UniquePipeline& pl,
            const QString& fragShaderPath);

    void loadShadersFromDisk();
    void createComputePipelines();
    vk::UniquePipeline createComputePipeline(NodeType nodeType);
    vk::UniquePipeline createComputePipelineNoop();

    // Load image
    bool createImageFromFile(
            const QString &path,
            const int colorSpace);
    bool writeLinearImage(
            float* imgStart,
            QSize imgSize,
            std::unique_ptr<CsImage>& image);

    // Compute setup
    void createComputePipelineLayout();
    void createComputeQueue();
    void createComputeCommandPool();
    void createQueryPool();

    // Recurring compute
    vk::UniqueShaderModule createShaderFromFile(
            const QString &name);

    bool createComputeRenderTarget(
            uint32_t width,
            uint32_t height);

    void createComputeDescriptors();
    void updateComputeDescriptors(
            const CsImage* const inputImageBack,
            const CsImage* const inputImageFront,
            const CsImage* const outputImage);
    void createComputeCommandBuffers();
    void recordComputeCommandBufferImageLoad(
            CsImage* const outputImage);
    void recordComputeCommandBufferGeneric(
            CsImage* const inputImageBack,
            CsImage* const inputImageFront,
            CsImage* const outputImage,
            vk::Pipeline& pl,
            int numShaderPasses,
            int currentPass);
    void recordComputeCommandBufferCPUCopy(
            CsImage* const inputImage);

    void submitComputeCommands();
    void submitImageSaveCommand();

    // Has to be called in startNextFrame()
    void createRenderPass();

    void updateVertexData(
            const int w,
            const int h);

    QString lookupColorSpace(
            const int i);
    void transformColorSpace(
            const QString& from,
            const QString& to,
            ImageBuf& image);

    void createBuffer(
            vk::UniqueBuffer& buffer,
            vk::UniqueDeviceMemory& bufferMemory,
            vk::DeviceSize& size);

    void fillSettingsBuffer(
            const NodeBase* node);

    void logicalDeviceLost() override;

    std::vector<float> unpackPushConstants(
            const QString& s);

    uint32_t findMemoryType(
            uint32_t typeFilter,
            vk::MemoryPropertyFlags properties);

    VulkanWindow *window;
    vk::Device device;
    vk::PhysicalDevice physicalDevice;

    vk::UniqueBuffer vertexBuffer;
    vk::UniqueDeviceMemory vertexBufferMemory;
    vk::DescriptorBufferInfo uniformBufferInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    vk::UniqueDescriptorPool descriptorPool;
    vk::UniqueDescriptorSetLayout graphicsDescriptorSetLayout;
    std::vector<vk::UniqueDescriptorSet> graphicsDescriptorSet;

    vk::UniquePipelineCache pipelineCache;
    vk::UniquePipelineLayout graphicsPipelineLayout;
    vk::UniquePipeline graphicsPipelineRGB;
    vk::UniquePipeline graphicsPipelineAlpha;
    vk::UniqueQueryPool queryPool;

    vk::UniqueSampler sampler;

    vk::UniqueBuffer outputStagingBuffer;
    vk::UniqueDeviceMemory outputStagingBufferMemory;

    vk::UniqueShaderModule noopShader;
    vk::UniquePipeline computePipelineNoop;

    bool texStagingPending = false;

    QSize loadImageSize;

    QSize currentRenderSize;

    std::unique_ptr<ImageBuf> cpuImage;
    QString imagePath;

    int concurrentFrameCount;  

    QSize outputImageSize;

    const vk::ClearColorValue clearColor = std::array<float, 4> ({ 0.05f, 0.05f, 0.05f, 0.0f });

    QMatrix4x4 projection;
    float rotation    = 0.0f;
    float position_x  = 0.0f;
    float position_y  = 0.0f;
    float position_z  = 0.0f;
    float scaleXY     = 1.0f;

    // TODO: Replace this with proper render states
    bool clearScreen = true;

    const vk::Format globalImageFormat = vk::Format::eR32G32B32A32Sfloat;
    DisplayMode displayMode = DISPLAY_MODE_RGB;

    // Compute resources
    struct Compute
    {
        vk::Queue                           computeQueue;
        vk::UniqueCommandPool               computeCommandPool;
        vk::UniqueCommandBuffer             commandBufferGeneric;     // Command buffer for all shaders except IO
        vk::UniqueCommandBuffer             commandBufferImageLoad;   // Command buffer for loading images from disk
        vk::UniqueCommandBuffer             commandBufferImageSave;   // Command buffer for writing images to disk
        vk::UniqueFence                     fence;
        uint32_t                            queueFamilyIndex;         // Family index of the graphics queue, used for barriers
    };

    Compute                                 compute;
    vk::UniquePipelineLayout                computePipelineLayoutGeneric;
    vk::UniquePipeline                      computePipeline;
    vk::UniqueDescriptorSetLayout           computeDescriptorSetLayoutGeneric;
    vk::UniqueDescriptorSet                 computeDescriptorSetGeneric;

    std::unique_ptr<CsImage>                computeRenderTarget;
    std::unique_ptr<CsImage>                imageFromDisk;
    std::unique_ptr<CsImage>                loadImageStaging;

    std::map<NodeType, vk::UniqueShaderModule>  shaders;
    std::map<NodeType, vk::UniquePipeline>      pipelines;

    std::vector<float> viewerPushConstants = { 0.0f, 1.0f, 1.0f };

    std::unique_ptr<CsSettingsBuffer> settingsBuffer;

    OCIO::ConstConfigRcPtr ocioConfig;

};

#endif // VULKANRENDERER_H
