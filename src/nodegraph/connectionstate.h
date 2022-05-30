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

#include <QtCore/QUuid>

#include "porttype.h"

class QPointF;

namespace Cascade::NodeGraph
{

class Node;

/// Stores currently draggind end.
/// Remembers last hovered Node.
class ConnectionState
{
public:
    ConnectionState(PortType port = PortType::None)
        : mRequiredPort(port)
    {}

    ConnectionState(const ConnectionState&) = delete;
    ConnectionState operator=(const ConnectionState&) = delete;

    ~ConnectionState();

public:
    void setRequiredPort(PortType end)
    { mRequiredPort = end; }

    PortType requiredPort() const
    { return mRequiredPort; }

    bool requiresPort() const
    { return mRequiredPort != PortType::None; }

    void setNoRequiredPort()
    { mRequiredPort = PortType::None; }

public:
    void interactWithNode(Node* node);

    void setLastHoveredNode(Node* node);

    Node* lastHoveredNode() const
    { return mLastHoveredNode; }

    void resetLastHoveredNode();

private:
    PortType mRequiredPort;

    Node* mLastHoveredNode { nullptr };
};
}
