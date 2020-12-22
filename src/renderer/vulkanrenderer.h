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

#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <array>

#include <QVulkanWindow>
#include <QImage>

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/color.h>
//#include <OpenColorIO/OpenColorIO.h>

#include <gmic.h>

#include "../nodedefinitions.h"
#include "../nodebase.h"
#include "../windowmanager.h"
#include "cssettingsbuffer.h"
#include "csimage.h"

using namespace Cascade;
using namespace OIIO;
//namespace OCIO = OCIO_NAMESPACE;

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
            std::shared_ptr<CsImage> inputImageBack,
            std::shared_ptr<CsImage> inputImageFront,
            const QSize targetSize);
    void processGmicNode(
            NodeBase *node,
            std::shared_ptr<CsImage> inputImageBack,
            const QSize targetSize);
    void displayNode(
            NodeBase* node);
    void doClearScreen();
    void setDisplayMode(
            DisplayMode mode);

    bool saveImageToDisk(
            CsImage& inputImage,
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
    void createVertexBuffer();
    void createSampler();
    void createDescriptorPool();
    void createGraphicsDescriptors();
    void createGraphicsPipelineCache();
    void createGraphicsPipelineLayout();
    void createGraphicsPipeline(
            VkPipeline& pl,
            VkShaderModule& fragShaderModule);

    void loadShadersFromDisk();
    void createComputePipelines();
    VkPipeline createComputePipeline(
            NodeType nodeType);
    VkPipeline createComputePipelineNoop();

    // Load image
    bool createTextureFromFile(
            const QString &path,
            const int colorSpace);
    bool createTextureFromGmic(
            gmic_image<float>& gImg);
    bool createTextureImage(
            const QSize &size,
            VkImage *image,
            VkDeviceMemory *mem,
            VkImageTiling tiling,
            VkImageUsageFlags usage,
            uint32_t memIndex);
    bool writeLinearImage(
            float* imgStart,
            QSize imgSize,
            VkImage image,
            VkDeviceMemory memory);
    bool writeGmicToLinearImage(
            float* imgStart,
            QSize imgSize,
            int channels,
            VkImage image,
            VkDeviceMemory memory);

    // Compute setup
    void createComputePipelineLayout();
    void createComputeQueue();
    void createComputeCommandPool();
    void createQueryPool();

    // Recurring compute
    VkShaderModule createShaderFromFile(
            const QString &name);

    bool createComputeRenderTarget(
            uint32_t width,
            uint32_t height);

    void createImageMemoryBarrier(
            VkImageMemoryBarrier& barrier,
            VkImageLayout targetLayout,
            VkAccessFlags srcMask,
            VkAccessFlags dstMask,
            CsImage& image);

    void createComputeDescriptors();
    void updateComputeDescriptors(
            std::shared_ptr<CsImage> inputImageBack,
            std::shared_ptr<CsImage> inputImageFront,
            std::shared_ptr<CsImage> outputImage);
    void createComputeCommandBuffers();
    void recordComputeCommandBufferImageLoad(
            std::shared_ptr<CsImage> outputImage);
    void recordComputeCommandBufferGeneric(
            std::shared_ptr<CsImage> inputImageBack,
            std::shared_ptr<CsImage> inputImageFront,
            std::shared_ptr<CsImage> outputImage,
            VkPipeline& pl,
            int numShaderPasses,
            int currentPass);
    void recordComputeCommandBufferCPUCopy(
            CsImage& inputImage);

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
            VkBuffer& buffer,
            VkDeviceMemory& bufferMemory,
            VkDeviceSize& size);

    void fillSettingsBuffer(
            NodeBase* node);

    void logicalDeviceLost() override;

    std::vector<float> unpackPushConstants(
            const QString s);

    uint32_t findMemoryType(
            uint32_t typeFilter,
            VkMemoryPropertyFlags properties);

    VulkanWindow *window;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    QVulkanDeviceFunctions *devFuncs;
    QVulkanFunctions *f;

    VkBuffer vertexBuffer                       = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory           = VK_NULL_HANDLE;
    VkDescriptorBufferInfo uniformBufferInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkDescriptorPool descriptorPool             = VK_NULL_HANDLE;
    VkDescriptorSetLayout graphicsDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet graphicsDescriptorSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkPipelineCache pipelineCache               = VK_NULL_HANDLE;
    VkPipelineLayout graphicsPipelineLayout     = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineRGB              = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineAlpha            = VK_NULL_HANDLE;
    VkQueryPool queryPool                       = VK_NULL_HANDLE;

    VkSampler sampler                           = VK_NULL_HANDLE;

    VkImage loadImageStaging                    = VK_NULL_HANDLE;
    VkDeviceMemory loadImageStagingMem          = VK_NULL_HANDLE;

    VkBuffer outputStagingBuffer                = VK_NULL_HANDLE;
    VkDeviceMemory outputStagingBufferMemory    = VK_NULL_HANDLE;

    VkShaderModule noopShader                   = VK_NULL_HANDLE;
    VkPipeline computePipelineNoop              = VK_NULL_HANDLE;

    bool texStagingPending = false;

    QSize loadImageSize;

    QSize currentRenderSize;

    std::unique_ptr<ImageBuf> cpuImage;
    QString imagePath;

    int concurrentFrameCount;  

    QSize outputImageSize;

    VkClearColorValue clearColor = {{ 0.05f, 0.05f, 0.05f, 0.0f }};

    QMatrix4x4 projection;
    float rotation    = 0.0f;
    float position_x  = 0.0f;
    float position_y  = 0.0f;
    float position_z  = 0.0f;
    float scaleXY     = 1.0f;

    // TODO: Replace this with proper render states
    bool clearScreen = true;

    const VkFormat globalImageFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
    DisplayMode displayMode = DISPLAY_MODE_RGB;

    // Compute resources
    struct Compute
    {
        VkQueue                     computeQueue;
        VkCommandPool               computeCommandPool;
        VkCommandBuffer             commandBufferGeneric;     // Command buffer for all shaders except IO
        VkCommandBuffer             commandBufferImageLoad;   // Command buffer for loading images from disk
        VkCommandBuffer             commandBufferImageSave;   // Command buffer for writing images to disk
        VkFence                     fence;
        uint32_t                    queueFamilyIndex;         // Family index of the graphics queue, used for barriers
    };

    Compute                         compute;
    VkPipelineLayout                computePipelineLayoutGeneric      = VK_NULL_HANDLE;
    VkPipeline                      computePipeline                   = VK_NULL_HANDLE;
    VkDescriptorSetLayout           computeDescriptorSetLayoutGeneric = VK_NULL_HANDLE;
    VkDescriptorSet                 computeDescriptorSetGeneric       = VK_NULL_HANDLE;

    std::shared_ptr<CsImage>        computeRenderTarget               = nullptr;
    std::shared_ptr<CsImage>        imageFromDisk                     = nullptr;

    QMap<NodeType, VkShaderModule>  shaders;
    QMap<NodeType, VkPipeline>      pipelines;

    std::vector<float> viewerPushConstants = { 0.0f, 1.0f, 1.0f };

    std::unique_ptr<CsSettingsBuffer> settingsBuffer;

    //OCIO::ConstConfigRcPtr ocioConfig;

    std::shared_ptr<gmic> gmicInstance;
    //gmic_image<float> gmicImage;
    //gmic_list<float> gmicList;
    //gmic_list<char> gmicNames;

};

#endif // VULKANRENDERER_H
