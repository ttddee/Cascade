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

#ifndef RENDERUTILITY_H
#define RENDERUTILITY_H

#include <QString>
#include <QStringList>

#include <vulkan/vulkan.hpp>

namespace Cascade::Renderer
{

inline const VkDeviceSize aligned(VkDeviceSize v, VkDeviceSize byteAlign)
{
    return (v + byteAlign - 1) & ~(byteAlign - 1);
}

inline const std::vector<float> unpackPushConstants(const QString& s)
{
    std::vector<float> values;
    auto parts = s.split(",");
    foreach(const QString& part, parts)
    {
        values.push_back(part.toFloat());
    }
    return values;
}

inline const std::vector<char> uintVecToCharVec(const std::vector<unsigned int>& in)
{
    std::vector<char> out;

    for (size_t i = 0; i < in.size(); i++)
    {
        out.push_back(in[i] >> 0);
        out.push_back(in[i] >> 8);
        out.push_back(in[i] >> 16);
        out.push_back(in[i] >> 24);
    }

    return out;
}

} // end namespace Cascade::Renderer

#endif // RENDERUTILITY_H
