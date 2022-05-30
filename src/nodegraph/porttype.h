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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#pragma once

#include <utility>
#include <QtCore/QUuid>

namespace Cascade::NodeGraph
{

enum class PortType
{
    None,
    In,
    Out
};

static const int INVALID = -1;

using PortIndex = int;

struct Port
{
    PortType type;

    PortIndex index;

    Port()
        : type(PortType::None),
        index(INVALID)
    {}

    Port(PortType t, PortIndex i)
        : type(t),
        index(i)
    {}

    bool indexIsValid() { return index != INVALID; }

    bool portTypeIsValid() { return type != PortType::None; }
};

//using PortAddress = std::pair<QUuid, PortIndex>;

inline PortType oppositePort(PortType port)
{
    PortType result = PortType::None;

    switch (port)
    {
    case PortType::In:
        result = PortType::Out;
        break;

    case PortType::Out:
        result = PortType::In;
        break;

    default:
        break;
    }

    return result;
}
}
