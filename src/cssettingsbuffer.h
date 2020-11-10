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
