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
    kp::Manager manager;
};

} // end namespace Cascade::Renderer

#endif // COMPUTEMANAGER_H
