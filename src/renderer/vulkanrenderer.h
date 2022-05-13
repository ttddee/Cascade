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
#include "../nodegraph/nodedefinitions.h"
#include "../nodegraph/nodebase.h"
#include "../windowmanager.h"
#include "cssettingsbuffer.h"
#include "csimage.h"
#include "cscommandbuffer.h"

namespace OCIO = OCIO_NAMESPACE;

using OIIO::ImageBuf;

class VulkanWindow;

namespace Cascade::Renderer {

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

    VulkanWindow *mWindow;
    vk::Device mDevice;
    vk::PhysicalDevice mPhysicalDevice;

    vk::UniqueBuffer mVertexBuffer;
    vk::UniqueDeviceMemory mVertexBufferMemory;
    vk::DescriptorBufferInfo mUniformBufferInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    vk::UniqueDescriptorPool mDescriptorPool;
    vk::UniqueDescriptorSetLayout mGraphicsDescriptorSetLayout;
    std::vector<vk::UniqueDescriptorSet> mGraphicsDescriptorSet;

    vk::UniquePipelineCache mPipelineCache;
    vk::UniquePipelineLayout mGraphicsPipelineLayout;
    vk::UniquePipeline mGraphicsPipelineRGB;
    vk::UniquePipeline mGraphicsPipelineAlpha;
    vk::UniqueQueryPool mQueryPool;

    vk::UniqueSampler mSampler;

    vk::UniquePipeline mComputePipelineNoop;
    vk::UniqueShaderModule mShaderUser;
    vk::UniquePipeline mComputePipelineUser;

    QSize mCurrentRenderSize;

    std::unique_ptr<ImageBuf> mCpuImage;
    QString mImagePath;

    int mConcurrentFrameCount;

    QSize mOutputImageSize;

    QMatrix4x4 mProjection;
    float mRotation    = 0.0f;
    float mPositionX  = 0.0f;
    float mPositionY  = 0.0f;
    float mPositionZ  = 0.0f;
    float mScaleXY     = 1.0f;

    // TODO: Replace this with proper render states
    bool mClearScreen = true;

    DisplayMode mDisplayMode = DisplayMode::eRgb;

    std::unique_ptr<CsCommandBuffer> mComputeCommandBuffer;

    vk::UniquePipelineLayout                mComputePipelineLayout;
    vk::UniquePipeline                      mComputePipeline;
    vk::UniqueDescriptorSetLayout           mComputeDescriptorSetLayout;
    vk::UniqueDescriptorSet                 mComputeDescriptorSet;

    std::unique_ptr<CsImage>                mLoadImageStaging;
    std::unique_ptr<CsImage>                mTmpCacheImage;
    std::unique_ptr<CsImage>                mComputeRenderTarget;

    std::map<NodeType, vk::UniqueShaderModule>  mShaders;
    std::map<NodeType, vk::UniquePipeline>      mPipelines;

    // TODO: Move this out of here
    std::vector<float> mViewerPushConstants = { 0.0f, 0.5f, 0.0f, 1.0f, 1.0f };

    std::unique_ptr<CsSettingsBuffer> mSettingsBuffer;

    OCIO::ConstConfigRcPtr mOcioConfig;
};

} // end namespace Cascade::Renderer

#endif // VULKANRENDERER_H
