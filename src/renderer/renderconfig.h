/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RENDERCONFIG_H
#define RENDERCONFIG_H

#include <QString>
#include <QByteArrayList>

#include "vulkanhppinclude.h"

#define NON_DISPATCHABLE_HANDLE_TO_UINT64_CAST(type, x) reinterpret_cast<uint64_t>(static_cast<type>(x))

namespace Cascade::Renderer {

inline const QByteArrayList instanceLayers =
{
    "VK_LAYER_KHRONOS_validation"
};

inline const QByteArrayList instanceExtensions =
{
#ifdef QT_DEBUG
    "VK_EXT_debug_utils"
#endif
};

inline constexpr vk::Format globalImageFormat(vk::Format::eR32G32B32A32Sfloat);

inline const vk::ClearColorValue clearColor(std::array<float, 4>({ 0.05f, 0.05f, 0.05f, 0.0f }));

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

} // namespace Cascade::Renderer

#endif // RENDERCONFIG_H
