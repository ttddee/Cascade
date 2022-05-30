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

#include "nodestyle.h"
#include "connectionstyle.h"
#include "nodegraphviewstyle.h"

namespace Cascade::NodeGraph
{

class StyleCollection
{
public:
    static NodeStyle const&  nodeStyle();

    static ConnectionStyle const& connectionStyle();

    static NodeGraphViewStyle const& nodeGraphViewStyle();

public:
    static void setNodeStyle(NodeStyle);

    static void setConnectionStyle(ConnectionStyle);

    static void setnodeGraphViewStyle(NodeGraphViewStyle);

private:
    StyleCollection() = default;

    StyleCollection(StyleCollection const&) = delete;

    StyleCollection& operator=(StyleCollection const&) = delete;

    static StyleCollection& instance();

private:
    NodeStyle mNodeStyle;

    ConnectionStyle mConnectionStyle;

    NodeGraphViewStyle mNodeGraphViewStyle;
};
}
