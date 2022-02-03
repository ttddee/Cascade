#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMap>

namespace Cascade
{
enum ViewerMode
{
    VIEWER_MODE_FRONT_RGB,
    VIEWER_MODE_BACK_RGB,
    VIEWER_MODE_INPUT_ALPHA,
    VIEWER_MODE_OUTPUT_RGB,
    VIEWER_MODE_OUTPUT_ALPHA
};

static const QMap<ViewerMode, QString> viewerModeText =
{
    { VIEWER_MODE_FRONT_RGB, "RGB Front" },
    { VIEWER_MODE_BACK_RGB, "RGB Back" },
    { VIEWER_MODE_INPUT_ALPHA, "Input Alpha" },
    { VIEWER_MODE_OUTPUT_RGB, "RGB Out" },
    { VIEWER_MODE_OUTPUT_ALPHA, "Alpha Out" }
};

    namespace Renderer
    {

    }
}

#endif // GLOBAL_H
