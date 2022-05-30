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

#include "node.h"
#include "connection.h"

namespace Cascade::NodeGraph
{

class DataModelRegistry;
class NodeGraphScene;
class NodeDataModel;

/// Class performs various operations on the Node and Connection pair.
/// An instance should be created on the stack and destroyed when
/// the operation is completed
class NodeConnectionInteraction
{
public:
    NodeConnectionInteraction(
        Node& node,
        Connection& connection,
        NodeGraphScene& scene);

    /// Can connect when following conditions are met:
    /// 1) Connection 'requires' a port
    /// 2) Connection's vacant end is above the node port
    /// 3) Node port is vacant
    /// 4) Connection does not create a loop in the graph
    bool canConnect(PortIndex & portIndexr) const;

    /// 1)   Check conditions from 'canConnect'
    /// 1.5) If the connection is possible but a type conversion is needed, add a converter node to the scene, and connect it properly
    /// 2)   Assign node to required port in Connection
    /// 3)   Assign Connection to empty port in NodeState
    /// 4)   Adjust Connection geometry
    /// 5)   Poke model to initiate data transfer
    bool tryConnect() const;


    /// 1) Node and Connection should be already connected
    /// 2) If so, clear Connection entry in the NodeState
    /// 3) Propagate invalid data to IN node
    /// 4) Set Connection end to 'requiring a port'
    bool disconnect(PortType portToDisconnect) const;

private:

    PortType connectionRequiredPort() const;

    QPointF connectionEndScenePosition(PortType) const;

    QPointF nodePortScenePosition(
        PortType portType,
        PortIndex portIndex) const;

    PortIndex nodePortIndexUnderScenePoint(
        PortType portType,
        QPointF const &p) const;

    bool nodePortIsEmpty(PortType portType, PortIndex portIndex) const;

private:

    Node* mNode;

    Connection* mConnection;

    NodeGraphScene* mScene;
};
}
