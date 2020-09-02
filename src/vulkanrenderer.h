#ifndef VULKANRENDERER_H
#define VULKANRENDERER_H

#include <array>

#include <QVulkanWindow>
#include <QImage>

#include "globals.h"

using namespace Cascade;

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

    void updateImage(const QString& path);
    void updateShader(const ShaderCode& code);

    void translate(float dx, float dy);
    void scale(float s);

    QString getGpuName();

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
    void createGraphicsPipeline();

    // Load image
    bool createTexture(const QString &name);
    bool createTextureImage(const QSize &size, VkImage *image, VkDeviceMemory *mem,
                                VkImageTiling tiling, VkImageUsageFlags usage, uint32_t memIndex);
    bool writeLinearImage(const QImage &img, VkImage image, VkDeviceMemory memory);

    // Compute
    // ONCE
    void createComputePipelineLayout();
    void createComputeQueue();
    void createComputeCommandPool();
    void createQueryPool();

    // RECURRING
    // Load shader
    VkShaderModule createShaderFromFile(const QString &name); // TODO: Take this out
    VkShaderModule createShaderFromCode(const ShaderCode& code);

    bool createComputeRenderTarget( uint32_t width, uint32_t height);

    void createComputeDescriptors();
    void updateComputeDescriptors();
    void createComputeCommandBuffer();
    void createComputePipeline();
    void recordComputeCommandBuffer();

    // Called in startNextFrame()
    void submitComputeCommands();
    void createRenderPass();

    void updateVertexData(int, int);

    ShaderCode shaderCode;

    VkDeviceMemory bufMem = VK_NULL_HANDLE;
    VkBuffer buf = VK_NULL_HANDLE;
    VkDescriptorBufferInfo uniformBufInfo[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkDescriptorPool descPool = VK_NULL_HANDLE;
    VkDescriptorSetLayout descSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet descSet[QVulkanWindow::MAX_CONCURRENT_FRAME_COUNT];

    VkPipelineCache pipelineCache = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkQueryPool queryPool = VK_NULL_HANDLE;

    VkSampler sampler = VK_NULL_HANDLE;
    VkFormat texFormat;
    VkImage texImage = VK_NULL_HANDLE;
    VkDeviceMemory texMem = VK_NULL_HANDLE;
    bool texLayoutPending = false;
    VkImageView texView = VK_NULL_HANDLE;
    VkImage texStaging = VK_NULL_HANDLE;
    VkDeviceMemory texStagingMem = VK_NULL_HANDLE;
    bool texStagingPending = false;
    QSize texSize;

    QImage cpuImage;
    QString imagePath = ":/images/empty.jpg";

    int concurrentFrameCount;

    QMatrix4x4 projection;
    float rotation    = 0.0f;
    float position_x  = 0.0f;
    float position_y  = 0.0f;
    float position_z  = 0.0f;
    float scaleXY       = 1.0f;

    // Compute resources
    struct Compute
    {
        VkQueue             queue;                // Separate queue for compute commands (queue family may differ from the one used for graphics)
        VkCommandPool       commandPool;          // Use a separate command pool (queue family may differ from the one used for graphics)
        VkCommandBuffer     commandBuffer;        // Command buffer storing the dispatch commands and barriers
        VkCommandBuffer     commandBufferInit;    // Command buffer used only for initial initialization and transfering data accross the pci bus
        VkFence             fence;                // Synchronization fence to avoid rewriting compute CB if still in use
        uint32_t            queueFamilyIndex;     // Family index of the graphics queue, used for barriers
    };

    Compute                 compute;
    VkPipelineLayout        computePipelineLayout      = VK_NULL_HANDLE;
    VkPipeline              computePipeline            = VK_NULL_HANDLE;
    VkDescriptorSetLayout   computeDescriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet         computeDescriptorSet;

    VkDeviceMemory          computeRenderTargetMemory  = VK_NULL_HANDLE;
    VkImage                 computeRenderTarget        = VK_NULL_HANDLE;
    VkImageView             computeRenderTargetView    = VK_NULL_HANDLE;

};



#endif // VULKANRENDERER_H
