#ifndef COMPUTEMANAGER_H
#define COMPUTEMANAGER_H

#include <kompute/Kompute.hpp>

namespace Cascade::Renderer
{

class ComputeManager
{
public:
    ComputeManager();

private:
    std::unique_ptr<kp::Manager> manager;

    std::shared_ptr<vk::Instance> instance = nullptr;
    std::shared_ptr<vk::PhysicalDevice> physicalDevice = nullptr;
    std::shared_ptr<vk::Device> device = nullptr;
};

} // end namespace Cascade::Renderer

#endif // COMPUTEMANAGER_H
