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

static const QMap<ViewerMode, QString> viewerModeText =
{
    { ViewerMode::eFrontRgb, "RGB Front" },
    { ViewerMode::eBackRgb, "RGB Back" },
    { ViewerMode::eInputAlpha, "Input Alpha" },
    { ViewerMode::eOutputRgb, "RGB Out" },
    { ViewerMode::eOutputAlpha, "Alpha Out" }
};

    namespace Config
    {
        [[maybe_unused]] static constexpr ViewerMode defaultViewerMode = ViewerMode::eOutputRgb;

        [[maybe_unused]] static constexpr QColor red(229, 70, 61);      // Red
        [[maybe_unused]] static constexpr QColor green(53, 226, 87);    // Green
        [[maybe_unused]] static constexpr QColor blue(35, 114, 239);    // Blue

        // Node
        [[maybe_unused]] static constexpr int nodeCornerRadius(6);
        [[maybe_unused]] static constexpr QColor defaultNodeColor(24, 27, 30);
        [[maybe_unused]] static constexpr QColor selectedNodeColor(37, 74, 115);

    } // namespace Cascade::Config

    namespace Renderer
    {

    } // namespace Cascade::Renderer

} // namepsace Cascade

#endif // GLOBAL_H
