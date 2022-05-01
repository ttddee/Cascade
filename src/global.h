#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMap>
#include <QColor>
#include <QBrush>

namespace Cascade {

enum class DisplayMode
{
    eRgb,
    eAlpha
};

enum class ViewerMode
{
    eFrontRgb,
    eBackRgb,
    eInputAlpha,
    eOutputRgb,
    eOutputAlpha
};

static const QMap<ViewerMode, QString> sViewerModeText =
{
    { ViewerMode::eFrontRgb,        "RGB Front" },
    { ViewerMode::eBackRgb,         "RGB Back" },
    { ViewerMode::eInputAlpha,      "Alpha Input" },
    { ViewerMode::eOutputRgb,       "RGB Out" },
    { ViewerMode::eOutputAlpha,     "Alpha Out" }
};

    namespace Config
    {
        [[maybe_unused]] static constexpr ViewerMode sDefaultViewerMode = ViewerMode::eOutputRgb;

        [[maybe_unused]] static constexpr QColor sRed(229, 70, 61);
        [[maybe_unused]] static constexpr QColor sGreen(53, 226, 87);
        [[maybe_unused]] static constexpr QColor sBlue(35, 114, 239);

        // Node
        [[maybe_unused]] static constexpr int sNodeCornerRadius(6);
        [[maybe_unused]] static constexpr QColor sDefaultNodeColor(24, 27, 30);
        [[maybe_unused]] static constexpr QColor sSelectedNodeColor(37, 74, 115);

    } // namespace Cascade::Config

    namespace Renderer
    {

    } // namespace Cascade::Renderer

} // namepsace Cascade

#endif // GLOBAL_H
