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

    void processReadNode(
            NodeBase* node);
    void processNode(
            NodeBase* node,
            CsImage& inputImageBack,
            const QSize targetSize);
    void processNode(
            NodeBase* node,
            CsImage& inputImageBack,
            CsImage& inputImageFront,
            const QSize targetSize);
    void displayNode(NodeBase* node);
    void doClearScreen();
    void setDisplayMode(DisplayMode mode);

    void saveImageToDisk(CsImage& inputImage, const QString& path);

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
    void createDescriptorPool();
    void createGraphicsDescriptors();
    void createGraphicsPipelineCache();
    void createGraphicsPipelineLayout();
    void createGraphicsPipeline(VkPipeline& pl, const VkShaderModule& fragShaderModule);

    void loadShadersFromDisk();
    void createComputePipelines();
    VkPipeline createComputePipeline(NodeType nodeType);
    VkPipeline createComputePipelineNoop();

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
            CsImage& inputImageBack,
            CsImage& outputImage);
    void updateComputeDescriptors(
            CsImage& inputImageBack,
            CsImage& inputImageFront,
            CsImage& outputImage);
    void createComputeCommandBuffer();
    void recordComputeCommandBuffer(
            CsImage& inputImageBack,
            CsImage& outputImage,
            VkPipeline& pl);
    void recordComputeCommandBuffer(
            CsImage& inputImageBack,
            CsImage& inputImageFront,
            CsImage& outputImage,
            VkPipeline& pl);
    void recordComputeCommandBuffer(
            CsImage& inputImage,
            const QString& path);

    void submitComputeCommands();
    void submitImageSaveCommand();

    // Has to be called in startNextFrame()
    void createRenderPass();

    void updateVertexData(int, int);

    void logicalDeviceLost() override;

    std::vector<float> unpackPushConstants(const QString s);

    VkDeviceMemory bufMem = VK_NULL_HANDLE;
    VkBuffer buf = VK_NULL_HANDLE;
    VkDescriptorBufferInfo uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout graphicsDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet graphicsDescriptorSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineRGB = VK_NULL_HANDLE;
    VkPipeline graphicsPipelineAlpha = VK_NULL_HANDLE;
    VkQueryPool queryPool = VK_NULL_HANDLE;

    VkSampler sampler = VK_NULL_HANDLE;

    bool texLayoutPending = false;

    VkFormat loadImageFormat;
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

    // TODO: Replace this with proper render states
    bool clearScreen = true;
    bool renderTwoInputs = false;

    DisplayMode displayMode = DISPLAY_MODE_RGB;

    // Compute resources
    struct Compute
    {
        VkQueue                     queue;                // Separate queue for compute commands (queue family may differ from the one used for graphics)
        VkCommandPool               commandPool;          // Use a separate command pool (queue family may differ from the one used for graphics)
        VkCommandBuffer             commandBufferOneInput;    // Command buffer storing the dispatch commands and barriers
        VkCommandBuffer             commandBufferTwoInputs;
        VkCommandBuffer             commandBufferImageLoad;    // Command buffer used only for initial initialization and transfering data accross the pci bus
        VkCommandBuffer             commandBufferImageSave;
        VkFence                     fence;                // Synchronization fence to avoid rewriting compute CB if still in use
        uint32_t                    queueFamilyIndex;     // Family index of the graphics queue, used for barriers
    };

    Compute                         compute;
    VkPipelineLayout                computePipelineLayoutOneInput       = VK_NULL_HANDLE;
    VkPipelineLayout                computePipelineLayoutTwoInputs      = VK_NULL_HANDLE;
    VkPipeline                      computePipeline                     = VK_NULL_HANDLE;
    VkDescriptorSetLayout           computeDescriptorSetLayoutOneInput  = VK_NULL_HANDLE;
    VkDescriptorSetLayout           computeDescriptorSetLayoutTwoInputs = VK_NULL_HANDLE;
    VkDescriptorSet                 computeDescriptorSetOneInput        = VK_NULL_HANDLE;
    VkDescriptorSet                 computeDescriptorSetTwoInputs       = VK_NULL_HANDLE;

    std::unique_ptr<CsImage>        computeRenderTarget                 = nullptr;
    std::unique_ptr<CsImage>        imageFromDisk                       = nullptr;

    QMap<NodeType, VkShaderModule>  shaders;
    QMap<NodeType, VkPipeline>      pipelines;

    std::vector<float> pushConstants = { 1.0f };

};



#endif // VULKANRENDERER_H
