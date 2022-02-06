#include "computemanager.h"

namespace Cascade::Renderer
{

ComputeManager::ComputeManager()
{
    // initialize the vk::ApplicationInfo structure
    vk::ApplicationInfo applicationInfo;
    applicationInfo.pApplicationName = "Cascade";
    applicationInfo.pEngineName = "Kompute";
    applicationInfo.apiVersion = VK_API_VERSION_1_2;
    applicationInfo.engineVersion = VK_API_VERSION_1_2;
    applicationInfo.applicationVersion = VK_API_VERSION_1_2;

    // initialize the vk::InstanceCreateInfo
    vk::InstanceCreateInfo instanceCreateInfo;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;

    // create an Instance
    instance = std::make_shared<vk::Instance>(vk::createInstance(instanceCreateInfo));

    physicalDevice = std::make_shared<vk::PhysicalDevice>(instance->enumeratePhysicalDevices().front());

    // get the QueueFamilyProperties of the first PhysicalDevice
    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice->getQueueFamilyProperties();

    // get the first index into queueFamiliyProperties which supports graphics
    auto propertyIterator = std::find_if(
      queueFamilyProperties.begin(), queueFamilyProperties.end(), []( vk::QueueFamilyProperties const & qfp ) {
        return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
      } );
    size_t graphicsQueueFamilyIndex = std::distance( queueFamilyProperties.begin(), propertyIterator );
    assert( graphicsQueueFamilyIndex < queueFamilyProperties.size() );

    // create a Device
    float                     queuePriority = 0.0f;
    vk::DeviceQueueCreateInfo deviceQueueCreateInfo(
      vk::DeviceQueueCreateFlags(), static_cast<uint32_t>( graphicsQueueFamilyIndex ), 1, &queuePriority );
    device = std::make_shared<vk::Device>(physicalDevice->createDevice( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo)));

    manager = std::unique_ptr<kp::Manager>(new kp::Manager(instance, physicalDevice, device));
}

} // end namespace Cascade::Renderer
