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
#include <QtWidgets/QGraphicsScene>

#include <unordered_map>
#include <tuple>
#include <functional>

#include "quuidstdhash.h"
#include "datamodelregistry.h"
#include "memory.h"
#include "nodegraphdatamodel.h"

namespace Cascade::NodeGraph
{

class NodeDataModel;
class FlowItemInterface;
class Node;
class NodeGraphicsObject;
class Connection;
class ConnectionGraphicsObject;
class NodeStyle;

/// Scene holds connections and nodes.
class NodeGraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    NodeGraphScene(std::shared_ptr<DataModelRegistry> registry,
              QObject * parent = Q_NULLPTR);

    NodeGraphScene(QObject * parent = Q_NULLPTR);

    ~NodeGraphScene();

public:
    void setModel(std::unique_ptr<NodeGraphDataModel> model);

    std::shared_ptr<Connection> createConnection(
        PortType connectedPort,
        Node& node,
        PortIndex portIndex);

    std::shared_ptr<Connection> createConnection(
        Node& nodeIn,
        PortIndex portIndexIn,
        Node& nodeOut,
        PortIndex portIndexOut);

    std::shared_ptr<Connection> restoreConnection(QJsonObject const &connectionJson);

    void deleteConnection(Connection const& connection);

    Node& createNode(std::unique_ptr<NodeDataModel> && dataModel);

    Node& restoreNode(QJsonObject const& nodeJson);

    void removeNode(Node& node);

    DataModelRegistry&registry() const;

    void setRegistry(std::shared_ptr<DataModelRegistry> registry);

    void iterateOverNodes(std::function<void(Node*)> const & visitor);

    void iterateOverNodeData(std::function<void(NodeDataModel*)> const & visitor);

    void iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel*)> const & visitor);

    QPointF getNodePosition(Node const& node) const;

    void setNodePosition(Node& node, QPointF const& pos) const;

    QSizeF getNodeSize(Node const& node) const;

public:
    std::unordered_map<QUuid, std::unique_ptr<Node> > const & nodes() const;

    std::unordered_map<QUuid, std::shared_ptr<Connection> > const & connections() const;

    std::vector<Node*> allNodes() const;

    std::vector<Node*> selectedNodes() const;

public:
    void clearScene();

    void save() const;

    void load();

    QByteArray saveToMemory() const;

    void loadFromMemory(const QByteArray& data);

Q_SIGNALS:
    /**
   * @brief Node has been created but not on the scene yet.
   * @see nodePlaced()
   */
    void nodeCreated(Cascade::NodeGraph::Node &n);

    /**
   * @brief Node has been added to the scene.
   * @details Connect to this signal if need a correct position of node.
   * @see nodeCreated()
   */
    void nodePlaced(Cascade::NodeGraph::Node &n);

    void nodeDeleted(Cascade::NodeGraph::Node &n);

    void connectionCreated(Cascade::NodeGraph::Connection const &c);
    void connectionDeleted(Cascade::NodeGraph::Connection const &c);

    void nodeMoved(Cascade::NodeGraph::Node& n, const QPointF& newLocation);

    void nodeDoubleClicked(Cascade::NodeGraph::Node* n);

    void nodeClicked(Cascade::NodeGraph::Node& n);

    void connectionHovered(Cascade::NodeGraph::Connection& c, QPoint screenPos);

    void nodeHovered(Cascade::NodeGraph::Node& n, QPoint screenPos);

    void connectionHoverLeft(Cascade::NodeGraph::Connection& c);

    void nodeHoverLeft(Cascade::NodeGraph::Node& n);

    void nodeContextMenu(Cascade::NodeGraph::Node& n, const QPointF& pos);

private:
    std::unique_ptr<NodeGraphDataModel> mModel;

    using SharedConnection = std::shared_ptr<Connection>;
    using UniqueNode       = std::unique_ptr<Node>;

    // DO NOT reorder this member to go after the others.
    // This should outlive all the connections and nodes of
    // the graph, so that nodes can potentially have pointers into it,
    // which is why it comes first in the class.
    std::shared_ptr<DataModelRegistry> mRegistry;

    std::unordered_map<QUuid, SharedConnection> mConnections;
    std::unordered_map<QUuid, UniqueNode>       mNodes;

private Q_SLOTS:
    void setupConnectionSignals(Cascade::NodeGraph::Connection const& c);

    void sendConnectionCreatedToNodes(Cascade::NodeGraph::Connection const& c);
    void sendConnectionDeletedToNodes(Cascade::NodeGraph::Connection const& c);

};

Node* locateNodeAt(
    QPointF scenePoint, NodeGraphScene &scene,
    QTransform const & viewTransform);
}
