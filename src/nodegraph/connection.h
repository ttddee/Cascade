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

#include <QtCore/QObject>
#include <QtCore/QUuid>
#include <QtCore/QVariant>

#include "porttype.h"
#include "nodedata.h"

#include "serializable.h"
#include "connectionstate.h"
#include "connectiongeometry.h"
#include "quuidstdhash.h"
#include "memory.h"

namespace Cascade::NodeGraph
{

class Node;
class NodeData;
class ConnectionGraphicsObject;

class Connection
    : public QObject,
      public Serializable
{
    Q_OBJECT

public:

    /// New Connection is attached to the port of the given Node.
    /// The port has parameters (portType, portIndex).
    /// The opposite connection end will require another port.
    Connection(PortType portType,
               Node& node,
               PortIndex portIndex);

    Connection(Node& nodeIn,
               PortIndex portIndexIn,
               Node& nodeOut,
               PortIndex portIndexOut);

    Connection(const Connection&) = delete;
    Connection operator=(const Connection&) = delete;

    ~Connection();

public:
    QJsonObject save() const override;

public:
    QUuid id() const;

    /// Remembers the end being dragged.
    /// Invalidates Node address.
    /// Grabs mouse.
    void setRequiredPort(PortType portType);
    PortType requiredPort() const;

    void setGraphicsObject(std::unique_ptr<ConnectionGraphicsObject>&& graphics);

    /// Assigns a node to the required port.
    /// It is assumed that there is a required port, no extra checks
    void setNodeToPort(
        Node& node,
        PortType portType,
        PortIndex portIndex);

    void removeFromNodes() const;

public:
    ConnectionGraphicsObject& getConnectionGraphicsObject() const;

    ConnectionState const& connectionState() const;
    ConnectionState& connectionState();

    ConnectionGeometry& connectionGeometry();

    ConnectionGeometry const& connectionGeometry() const;

    Node* getNode(PortType portType) const;

    Node*& getNode(PortType portType);

    PortIndex getPortIndex(PortType portType) const;

    void clearNode(PortType portType);

    NodeDataType dataType(PortType portType) const;

    bool isComplete() const;

public: // data propagation
    void propagateData(std::shared_ptr<NodeData> nodeData) const;

    void propagateEmptyData() const;

Q_SIGNALS:
    void connectionCompleted(Cascade::NodeGraph::Connection const&);

    void connectionMadeIncomplete(Cascade::NodeGraph::Connection const&);

private:

    QUuid mUid;

private:

    Node* mOutNode = nullptr;
    Node* mInNode  = nullptr;

    PortIndex mOutPortIndex;
    PortIndex mInPortIndex;

private:

    ConnectionState    mConnectionState;
    ConnectionGeometry mConnectionGeometry;

    std::unique_ptr<ConnectionGraphicsObject> mConnectionGraphicsObject;

Q_SIGNALS:

    void updated(Cascade::NodeGraph::Connection& conn);
};
}
