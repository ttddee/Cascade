#ifndef RENDERTASKREAD_H
#define RENDERTASKREAD_H

#include "rendertask.h"

namespace Cascade::Renderer
{

class RenderTaskRead : public RenderTask
{
public:
    RenderTaskRead();

    void initialize(std::vector<PropertyData*> data) override;

    void execute() override;
};

} // namespace Cascade::Renderer

#endif // RENDERTASKREAD_H
