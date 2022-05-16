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

        static const QMap<QString, QPair<int, int>> sImageSizePresets =
        {
            { "NTSC",           { 720, 486 } },
            { "PAL",            { 720, 576 } },
            { "HD 720p",        { 1280, 720 } },
            { "HD 1080p",       { 1920, 1080 } },
            { "2K",             { 2048, 1024 } },
            { "4K",             { 4096, 2160 } },
            { "256 Square",     { 256, 256 } },
            { "512 Square",     { 512, 512 } },
            { "1K Square",      { 1024, 1024 } },
            { "2K Square",      { 2048, 2048 } }
        };

    } // namespace Cascade::Config

    namespace Renderer
    {

    } // namespace Cascade::Renderer

} // namepsace Cascade

#endif // GLOBAL_H
