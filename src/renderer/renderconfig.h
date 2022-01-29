#ifndef RENDERCONFIG_H
#define RENDERCONFIG_H

#include <QString>

#include <vulkan/vulkan.hpp>

namespace Cascade::Renderer
{
    inline constexpr vk::Format globalImageFormat(vk::Format::eR32G32B32A32Sfloat);

    inline const vk::ClearColorValue clearColor(std::array<float, 4>(
                                              { 0.05f, 0.05f, 0.05f, 0.0f }));

    inline constexpr int uniformDataSize = 16 * sizeof(float);

    inline const std::unordered_map<int, QString> colorSpaces =
    {
        { 0, "sRGB" },
        { 1, "linear" },
        { 2, "rec709" },
        { 3, "Gamma1.8" },
        { 4, "Gamma2.2" },
        { 5, "Panalog" },
        { 6, "REDLog" },
        { 7, "ViperLog" },
        { 8, "AlexaV3LogC" },
        { 9, "PLogLin" },
        { 10, "SLog" },
        { 11, "raw" }
    };

} // end namespace Cascade::Renderer

#endif // RENDERCONFIG_H
