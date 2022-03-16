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

#include "renderconfig.h"
#include "../nodedefinitions.h"
#include "../nodebase.h"
#include "../windowmanager.h"
#include "cssettingsbuffer.h"
#include "csimage.h"
#include "cscommandbuffer.h"

using namespace Cascade;
using namespace OIIO;
namespace OCIO = OCIO_NAMESPACE;

class VulkanWindow;

namespace Cascade::Renderer
{

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
    bool saveImageToDisk(
            CsImage* const inputImage,
            const QString& path,
            const QMap<std::string, std::string>& attributes,
            const int colorSpace);
    void displayNode(
            const NodeBase* node);
    void doClearScreen();
    void setDisplayMode(
            const DisplayMode mode);

    void setViewerPushConstants(const QString& s);

    void startNextFrame() override;

    QString getGpuName();

    void translate(float dx, float dy);
    void scale(float s);

    void shutdown();

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
            vk::UniquePipeline& pl,
            const QString& fragShaderPath);

    void loadShadersFromDisk();
    void createComputePipelines();
    vk::UniquePipeline createComputePipeline(const vk::ShaderModule& shaderModule);

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
    void createQueryPool();

    // Recurring compute
    vk::UniqueShaderModule createShaderFromFile(
            const QString &name);
    vk::UniqueShaderModule createShaderFromCode(
            const std::vector<unsigned int>& code);

    bool createComputeRenderTarget(
            uint32_t width,
            uint32_t height);

    void createComputeDescriptors();
    void updateGraphicsDescriptors(
            const CsImage* const outputImage,
            const CsImage* const upstreamImage);
    void updateComputeDescriptors(
            const CsImage* const inputImageBack,
            const CsImage* const inputImageFront,
            const CsImage* const outputImage);

    // Has to be called in startNextFrame()
    void createRenderPass();

    void updateVertexData(
            const int w,
            const int h);

    void transformColorSpace(
            const QString& from,
            const QString& to,
            ImageBuf& image);

    void fillSettingsBuffer(
            const NodeBase* node);

    void logicalDeviceLost() override;

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

    vk::UniquePipeline computePipelineNoop;
    vk::UniqueShaderModule shaderUser;
    vk::UniquePipeline computePipelineUser;

    QSize currentRenderSize;

    std::unique_ptr<ImageBuf> cpuImage;
    QString imagePath;

    int concurrentFrameCount;  

    QSize outputImageSize;

    QMatrix4x4 projection;
    float rotation    = 0.0f;
    float position_x  = 0.0f;
    float position_y  = 0.0f;
    float position_z  = 0.0f;
    float scaleXY     = 1.0f;

    // TODO: Replace this with proper render states
    bool clearScreen = true;

    DisplayMode displayMode = DISPLAY_MODE_RGB;

    std::unique_ptr<CsCommandBuffer> computeCommandBuffer;

    vk::UniquePipelineLayout                computePipelineLayout;
    vk::UniquePipeline                      computePipeline;
    vk::UniqueDescriptorSetLayout           computeDescriptorSetLayout;
    vk::UniqueDescriptorSet                 computeDescriptorSet;

    std::unique_ptr<CsImage>                loadImageStaging;
    std::unique_ptr<CsImage>                tmpCacheImage;
    std::unique_ptr<CsImage>                computeRenderTarget;

    std::map<NodeType, vk::UniqueShaderModule>  shaders;
    std::map<NodeType, vk::UniquePipeline>      pipelines;

    // TODO: Move this out of here
    std::vector<float> viewerPushConstants = { 0.0f, 0.5f, 0.0f, 1.0f, 1.0f };

    std::unique_ptr<CsSettingsBuffer> settingsBuffer;

    OCIO::ConstConfigRcPtr ocioConfig;
};

} // end namespace Cascade::Renderer

#endif // VULKANRENDERER_H
