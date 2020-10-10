#ifndef CSIMAGE_H
#define CSIMAGE_H

#include <QVulkanDeviceFunctions>

#include <vulkan/vulkan.h>

#include "vulkanwindow.h"

class CsImage
{
public:
    CsImage(
            VulkanWindow* win,
            const VkDevice* d,
            QVulkanDeviceFunctions* df,
            const int w = 100,
            const int h = 100);

    VkImage& getImage();
    VkImageView& getImageView();
    VkDeviceMemory& getMemory();

    int getWidth() const;
    int getHeight() const;

    void destroy();

    ~CsImage();

private:
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VulkanWindow* window;
    QVulkanDeviceFunctions *devFuncs;
    const VkDevice* device;

    const int width;
    const int height;
};

#endif // CSIMAGE_H
