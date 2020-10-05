#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <array>

#include <QVulkanWindow>
#include <QImage>

#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <OpenImageIO/color.h>

#include "nodedefinitions.h"
#include "csimage.h"
#include "nodebase.h"
#include "windowmanager.h"

using namespace Cascade;
using namespace OIIO;

class VulkanWindow;

class VulkanRenderer : public QVulkanWindowRenderer
{
public:
    VulkanRenderer(VulkanWindow *w);
    ~VulkanRenderer();

    // Initialize
    void initResources() override;
    void initSwapChainResources() override;
    void releaseSwapChainResources() override;
    void releaseResources() override;

    void startNextFrame() override;

    void translate(float dx, float dy);
    void scale(float s);

    QString getGpuName();

    void processNode(
            NodeBase* node,
            CsImage& inputImage,
            const QSize targetSize);
    void displayProcessedNode(NodeBase* node);
    void doClearScreen();

private:
    // Setup
    VulkanWindow *window;
    VkDevice device;
    VkPhysicalDevice physicalDevice;
    QVulkanDeviceFunctions *devFuncs;
    QVulkanFunctions *f;

    // Initialize
    void createVertexBuffer();
    void createSampler();
    void createGraphicsDescriptors();
    void createGraphicsPipelineCache();
    void createGraphicsPipelineLayout();
    void createGraphicsPipeline(VkPipeline& pl, const VkShaderModule& fragShaderModule);

    void loadShadersFromDisk();
    void createComputePipelines();
    VkPipeline createComputePipeline(const VkShaderModule& shaderModule);

    int getDeviceMemorySize();

    // Load image
    bool createTextureFromFile(const QString &path);
    bool createTextureImage(const QSize &size, VkImage *image, VkDeviceMemory *mem,
                                VkImageTiling tiling, VkImageUsageFlags usage, uint32_t memIndex);
    bool writeLinearImage(const ImageBuf &img, VkImage image, VkDeviceMemory memory);

    // Compute
    // ONCE
    void createComputePipelineLayout();
    void createComputeQueue();
    void createComputeCommandPool();
    void createQueryPool();

    // RECURRING
    // Load shader
    VkShaderModule createShaderFromFile(const QString &name);

    bool createComputeRenderTarget( uint32_t width, uint32_t height);

    void createComputeDescriptors();
    void updateComputeDescriptors(
            CsImage& inputImage,
            CsImage& outputImage);
    void createComputeCommandBuffer();
    void recordComputeCommandBuffer(
            CsImage& inputImage,
            CsImage& outputImage,
            VkPipeline& pl);

    //////////////////////


    // Called in startNextFrame()
    void submitComputeCommands();
    void createRenderPass();

    void updateVertexData(int, int);

    std::vector<float> unpackPushConstants(const QString s);

    VkDeviceMemory bufMem = VK_NULL_HANDLE;
    VkBuffer buf = VK_NULL_HANDLE;
    VkDescriptorBufferInfo uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkDescriptorPool descPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout descSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineRGB = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineAlpha = VK_NULL_HANDLE;
    VkQueryPool queryPool = VK_NULL_HANDLE;

    VkSampler sampler = VK_NULL_HANDLE;

    VkFormat loadImageFormat;
    VkDeviceMemory loadImageMem = VK_NULL_HANDLE;

    bool texLayoutPending = false;

    VkImage loadImageStaging = VK_NULL_HANDLE;
    VkDeviceMemory loadImageStagingMem = VK_NULL_HANDLE;

    bool texStagingPending = false;

    QSize loadImageSize;

    QSize currentRenderSize;

    std::unique_ptr<ImageBuf> cpuImage;
    QString imagePath = "../../images/empty.jpg";

    VkShaderModule noopShader;
    VkPipeline noopPipeline;

    int concurrentFrameCount;

    QMatrix4x4 projection;
    float rotation    = 0.0f;
    float position_x  = 0.0f;
    float position_y  = 0.0f;
    float position_z  = 0.0f;
    float scaleXY     = 1.0f;

    bool clearScreen = true;

    // Compute resources
    struct Compute
    {
        VkQueue                     queue;                // Separate queue for compute commands (queue family may differ from the one used for graphics)
        VkCommandPool               commandPool;          // Use a separate command pool (queue family may differ from the one used for graphics)
        VkCommandBuffer             commandBuffer;        // Command buffer storing the dispatch commands and barriers
        VkCommandBuffer             commandBufferImageLoad;    // Command buffer used only for initial initialization and transfering data accross the pci bus
        VkFence                     fence;                // Synchronization fence to avoid rewriting compute CB if still in use
        uint32_t                    queueFamilyIndex;     // Family index of the graphics queue, used for barriers
    };

    Compute                         compute;
    VkPipelineLayout                computePipelineLayout      = VK_NULL_HANDLE;
    VkPipeline                      computePipeline            = VK_NULL_HANDLE;
    VkDescriptorSetLayout           computeDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet                 computeDescriptorSet       = VK_NULL_HANDLE;

    std::unique_ptr<CsImage>        computeRenderTarget        = nullptr;
    std::unique_ptr<CsImage>        imageFromDisk              = nullptr;

    QMap<NodeType, VkShaderModule>  shaders;
    QMap<NodeType, VkPipeline>      pipelines;

    std::vector<float> pushConstants = { 1.0f };

};



#endif // VULKANRENDERER_H
