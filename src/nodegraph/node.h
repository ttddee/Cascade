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

#include <QtCore/QJsonObject>

#include "porttype.h"

#include "nodestate.h"
#include "nodegeometry.h"
#include "nodedata.h"
#include "nodegraphicsobject.h"
#include "connectiongraphicsobject.h"
#include "serializable.h"
#include "memory.h"
#include "../properties/propertywidget.h"

using Cascade::Properties::PropertyWidget;

namespace Cascade::NodeGraph
{

class Connection;
class ConnectionState;
class NodeGraphicsObject;
class NodeDataModel;

class Node
    : public QObject,
      public Serializable
{
    Q_OBJECT

public:
    /// NodeDataModel should be an rvalue and is moved into the Node
    Node(std::unique_ptr<NodeDataModel>&& dataModel);

    virtual ~Node();

public:
    QJsonObject save() const override;

    void restore(QJsonObject const &json) override;

public:
    QUuid id() const;

    void reactToPossibleConnection(
        PortType,
        NodeDataType const &,
        QPointF const & scenePoint);

    void resetReactionToConnection();

public:
    NodeGraphicsObject const& nodeGraphicsObject() const;

    NodeGraphicsObject& nodeGraphicsObject();

    void setGraphicsObject(std::unique_ptr<NodeGraphicsObject> graphics);

    NodeGeometry& nodeGeometry();

    NodeGeometry const& nodeGeometry() const;

    NodeState const& nodeState() const;

    NodeState& nodeState();

    NodeDataModel* nodeDataModel() const;

    PropertyWidget* propertyWidget();

public Q_SLOTS: // data propagation
    /// Propagates incoming data to the underlying model.
    void propagateData(
        std::shared_ptr<NodeData> nodeData,
        Cascade::NodeGraph::PortIndex inPortIndex,
        const QUuid& connectionId) const;

    /// Fetches data from model's OUT #index port
    /// and propagates it to the connection
    void onDataUpdated(Cascade::NodeGraph::PortIndex index);

    /// update the graphic part if the size of the embeddedwidget changes
    void onNodeSizeUpdated();

private:
    // addressing
    QUuid mUid;

    // data
    std::unique_ptr<NodeDataModel> mNodeDataModel;

    NodeState mNodeState;

    // painting
    NodeGeometry mNodeGeometry;

    std::unique_ptr<NodeGraphicsObject> mNodeGraphicsObject;

    PropertyWidget* mPropertyWidget = nullptr;
};
}
