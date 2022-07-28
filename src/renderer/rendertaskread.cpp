#include "rendertaskread.h"

#include "../log.h"

namespace Cascade::Renderer
{

RenderTaskRead::RenderTaskRead() {}

void RenderTaskRead::initialize( [[maybe_unused]] std::vector<PropertyData*> data) {}

void RenderTaskRead::execute()
{
    CS_LOG_INFO("Exec");
}

} // namespace Cascade::Renderer
