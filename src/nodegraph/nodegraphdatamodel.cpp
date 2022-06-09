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

#include "nodegraphdatamodel.h"

namespace Cascade::NodeGraph
{

NodeGraphDataModel::NodeGraphDataModel(
    NodeGraphScene* scene,
    QObject *parent) :
    QObject(parent)
{
    mRegistry = registerDataModels();

    mScene = scene;

    // This connection should come first
    connect(this, &NodeGraphDataModel::connectionCreated,
            this, &NodeGraphDataModel::setupConnectionSignals);
    connect(this, &NodeGraphDataModel::connectionCreated,
            this, &NodeGraphDataModel::sendConnectionCreatedToNodes);
    connect(this, &NodeGraphDataModel::connectionDeleted,
            this, &NodeGraphDataModel::sendConnectionDeletedToNodes);
}

NodeGraphData* NodeGraphDataModel::getData() const
{
    return mData.get();
}

std::shared_ptr<Connection> NodeGraphDataModel::createConnection(
    PortType connectedPort,
    Node& node,
    PortIndex portIndex)
{
    auto connection = std::make_shared<Connection>(connectedPort, node, portIndex);

    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *mScene, *connection);

    // after this function connection points are set to node port
    connection->setGraphicsObject(std::move(cgo));

    mData->addConnection(connection);
    //mConnections[connection->id()] = connection;

    // Note: this connection isn't truly created yet. It's only partially created.
    // Thus, don't send the connectionCreated(...) signal.

    connect(connection.get(), &Connection::connectionCompleted,
        this, [this](Connection const& c)
        {
            emit connectionCreated(c);
        });

    return connection;
}


std::shared_ptr<Connection> NodeGraphDataModel::createConnection(
    Node& nodeIn,
    PortIndex portIndexIn,
    Node& nodeOut,
    PortIndex portIndexOut)
{
    auto connection = std::make_shared<Connection>(
        nodeIn,
        portIndexIn,
        nodeOut,
        portIndexOut);

    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *mScene, *connection);

    nodeIn.nodeState().setConnection(PortType::In, portIndexIn, *connection);
    nodeOut.nodeState().setConnection(PortType::Out, portIndexOut, *connection);

    // after this function connection points are set to node port
    connection->setGraphicsObject(std::move(cgo));

    // trigger data propagation
    nodeOut.onDataUpdated(portIndexOut);

    mData->addConnection(connection);
    //mConnections[connection->id()] = connection;

    emit connectionCreated(*connection);

    return connection;
}


std::shared_ptr<Connection> NodeGraphDataModel::restoreConnection(
    QJsonObject const &connectionJson)
{
    QUuid nodeInId  = QUuid(connectionJson["in_id"].toString());
    QUuid nodeOutId = QUuid(connectionJson["out_id"].toString());

    PortIndex portIndexIn  = connectionJson["in_index"].toInt();
    PortIndex portIndexOut = connectionJson["out_index"].toInt();

    auto nodeIn  = mData->getNode(nodeInId);
    auto nodeOut = mData->getNode(nodeOutId);

    std::shared_ptr<Connection> connection =
        createConnection(*nodeIn, portIndexIn,
                         *nodeOut, portIndexOut);

    // Note: the connectionCreated(...) signal has already been sent
    // by createConnection(...)

    return connection;
}


void NodeGraphDataModel::deleteConnection(Connection const& connection)
{
//    auto it = mData->getConnection(connection.id());
//    if (it != mConnections.end())
//    {
//        connection.removeFromNodes();
//        mConnections.erase(it);
//    }
}

Node& NodeGraphDataModel::createNode(std::unique_ptr<NodeDataModel>&& dataModel)
{
    auto node = std::make_unique<Node>(std::move(dataModel));
    auto ngo  = std::make_unique<NodeGraphicsObject>(*this, *mScene, *node);

    node->setGraphicsObject(std::move(ngo));

    auto nodePtr = node.get();
    mData->mNodes[node->id()] = std::move(node);

    emit nodeCreated(*nodePtr);

    return *nodePtr;
}

Node& NodeGraphDataModel::restoreNode(QJsonObject const& nodeJson)
{
    QString modelName = nodeJson["model"].toObject()["name"].toString();

    auto dataModel = registry().create(modelName);

    if (!dataModel)
        throw std::logic_error(std::string("No registered model with name ") +
                               modelName.toLocal8Bit().data());

    auto node = std::make_unique<Node>(std::move(dataModel));
    auto ngo  = std::make_unique<NodeGraphicsObject>(*this, *mScene, *node);
    node->setGraphicsObject(std::move(ngo));

    node->restore(nodeJson);

    auto nodePtr = node.get();
    mData->addNode(std::move(node));

    emit nodePlaced(*nodePtr);

    emit nodeCreated(*nodePtr);

    return *nodePtr;
}


void NodeGraphDataModel::removeNode(Node& node)
{
    // call signal
//    emit nodeDeleted(node);

//    for(auto portType: {PortType::In,PortType::Out})
//    {
//        auto nodeState = node.nodeState();
//        auto const & nodeEntries = nodeState.getEntries(portType);

//        for (auto &connections : nodeEntries)
//        {
//            for (auto const &pair : connections)
//                deleteConnection(*pair.second);
//        }
//    }

//    mNodes.erase(node.id());
}


DataModelRegistry& NodeGraphDataModel::registry() const
{
    return *mRegistry;
}


void NodeGraphDataModel::setRegistry(std::shared_ptr<DataModelRegistry> registry)
{
    mRegistry = std::move(registry);
}


std::vector<Node*> NodeGraphDataModel::allNodes() const
{
    std::vector<Node*> nodes;

//    auto nodesMap = mData->getNodes();

//    std::transform(nodesMap.begin(),
//                   nodesMap.end(),
//                   std::back_inserter(nodes),
//                   [](std::pair<QUuid const, std::unique_ptr<Node>> const & p) { return p.second.get(); });

    return nodes;
}


void NodeGraphDataModel::iterateOverNodes(std::function<void(Node*)> const& visitor)
{
//    for (const auto& _node : mNodes)
//    {
//        visitor(_node.second.get());
//    }
}


void NodeGraphDataModel::iterateOverNodeData(std::function<void(NodeDataModel*)> const& visitor)
{
//    for (const auto& _node : mNodes)
//    {
//        visitor(_node.second->nodeDataModel());
//    }
}


void NodeGraphDataModel::iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel*)> const& visitor)
{
//    std::set<QUuid> visitedNodesSet;

//    //A leaf node is a node with no input ports, or all possible input ports empty
//    auto isNodeLeaf =
//        [](Node const &node, NodeDataModel const &model)
//    {
//        for (unsigned int i = 0; i < model.nPorts(PortType::In); ++i)
//        {
//            auto connections = node.nodeState().connections(PortType::In, i);
//            if (!connections.empty())
//            {
//                return false;
//            }
//        }

//        return true;
//    };

//    //Iterate over "leaf" nodes
//    for (auto const &_node : mNodes)
//    {
//        auto const &node = _node.second;
//        auto model       = node->nodeDataModel();

//        if (isNodeLeaf(*node, *model))
//        {
//            visitor(model);
//            visitedNodesSet.insert(node->id());
//        }
//    }

//    auto areNodeInputsVisitedBefore =
//        [&](Node const &node, NodeDataModel const &model)
//    {
//        for (size_t i = 0; i < model.nPorts(PortType::In); ++i)
//        {
//            auto connections = node.nodeState().connections(PortType::In, static_cast<PortIndex>(i));

//            for (auto& conn : connections)
//            {
//                if (visitedNodesSet.find(conn.second->getNode(PortType::Out)->id()) == visitedNodesSet.end())
//                {
//                    return false;
//                }
//            }
//        }

//        return true;
//    };

//    //Iterate over dependent nodes
//    while (mNodes.size() != visitedNodesSet.size())
//    {
//        for (auto const &_node : mNodes)
//        {
//            auto const &node = _node.second;
//            if (visitedNodesSet.find(node->id()) != visitedNodesSet.end())
//                continue;

//            auto model = node->nodeDataModel();

//            if (areNodeInputsVisitedBefore(*node, *model))
//            {
//                visitor(model);
//                visitedNodesSet.insert(node->id());
//            }
//        }
//    }
}


QPointF NodeGraphDataModel::getNodePosition(const Node& node) const
{
    return node.nodeGraphicsObject().pos();
}


void NodeGraphDataModel::setNodePosition(Node& node, const QPointF& pos) const
{
    node.nodeGraphicsObject().setPos(pos);
    node.nodeGraphicsObject().moveConnections();
}

void NodeGraphDataModel::setupConnectionSignals(Connection const& c)
{
    connect(&c, &Connection::connectionMadeIncomplete,
            this, &NodeGraphDataModel::connectionDeleted, Qt::UniqueConnection);
}

void NodeGraphDataModel::sendConnectionCreatedToNodes(Connection const& c)
{
    Node* from = c.getNode(PortType::Out);
    Node* to   = c.getNode(PortType::In);

    Q_ASSERT(from != nullptr);
    Q_ASSERT(to != nullptr);

    from->nodeDataModel()->outputConnectionCreated(c);
    to->nodeDataModel()->inputConnectionCreated(c);
}

void NodeGraphDataModel::sendConnectionDeletedToNodes(Connection const& c)
{
    Node* from = c.getNode(PortType::Out);
    Node* to   = c.getNode(PortType::In);

    Q_ASSERT(from != nullptr);
    Q_ASSERT(to != nullptr);

    from->nodeDataModel()->outputConnectionDeleted(c);
    to->nodeDataModel()->inputConnectionDeleted(c);
}

} //namespace Cascade::NodeGraph
