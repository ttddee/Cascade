#include "computemanager.h"

namespace Cascade::Renderer
{

ComputeManager::ComputeManager(
        vk::Instance* inst,
        vk::PhysicalDevice* physDev,
        vk::Device* dev)
{
    instance = std::shared_ptr<vk::Instance>(inst);
    physicalDevice = std::shared_ptr<vk::PhysicalDevice>(physDev);
    device = std::shared_ptr<vk::Device>(dev);
    manager = std::unique_ptr<kp::Manager>(new kp::Manager(instance, physicalDevice, device));
}

ComputeManager::ComputeManager()
{
    manager = std::unique_ptr<kp::Manager>(new kp::Manager());
}

} // end namespace Cascade::Renderer
