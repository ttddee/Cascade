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

#ifndef NODEGRAPHDATAMODEL_H
#define NODEGRAPHDATAMODEL_H

#include <QObject>

#include "nodegraphdata.h"
#include "datamodelregistry.h"

namespace Cascade::NodeGraph
{

class NodeGraphDataModel : public QObject
{
    Q_OBJECT

public:
    explicit NodeGraphDataModel(
        std::shared_ptr<DataModelRegistry> registry,
        NodeGraphScene* scene,
        QObject *parent = nullptr);

    NodeGraphData* getData() const;

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

    DataModelRegistry& registry() const;

    void setRegistry(std::shared_ptr<DataModelRegistry> registry);

    void iterateOverNodes(std::function<void(Node*)> const & visitor);

    void iterateOverNodeData(std::function<void(NodeDataModel*)> const & visitor);

    void iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel*)> const & visitor);

    QPointF getNodePosition(Node const& node) const;

    void setNodePosition(Node& node, QPointF const& pos) const;

    std::vector<Node*> allNodes() const;

private:
    std::shared_ptr<DataModelRegistry> mRegistry;

    std::unique_ptr<NodeGraphData> mData;
    NodeGraphScene* mScene;

signals:
    void connectionCreated(Cascade::NodeGraph::Connection const &c);

    void connectionDeleted(Cascade::NodeGraph::Connection const &c);

    void nodeCreated(Cascade::NodeGraph::Node &n);

    void nodePlaced(Cascade::NodeGraph::Node &n);

    void nodeDeleted(Cascade::NodeGraph::Node &n);

private slots:
    void setupConnectionSignals(Cascade::NodeGraph::Connection const& c);

    void sendConnectionCreatedToNodes(Cascade::NodeGraph::Connection const& c);

    void sendConnectionDeletedToNodes(Cascade::NodeGraph::Connection const& c);

};

} // namespace Cascade::NodeGraph

#endif // NODEGRAPHDATAMODEL_H
