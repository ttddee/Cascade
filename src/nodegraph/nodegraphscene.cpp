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

#include "nodegraphscene.h"

#include <stdexcept>
#include <utility>

#include <QtWidgets/QGraphicsSceneMoveEvent>
#include <QtWidgets/QFileDialog>
#include <QtCore/QByteArray>
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>
#include <QtCore/QFile>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>

#include "node.h"
#include "nodegraphicsobject.h"

#include "nodegraphicsobject.h"
#include "connectiongraphicsobject.h"

#include "connection.h"

#include "nodegraphview.h"
#include "datamodelregistry.h"

using Cascade::NodeGraph::NodeGraphScene;
using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeGraphicsObject;
using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::DataModelRegistry;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::NodeGraphDataModel;

NodeGraphScene::NodeGraphScene(QObject* parent) :
    QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);


}


NodeGraphScene::~NodeGraphScene()
{
    //clearScene();
}


//------------------------------------------------------------------------------

//std::shared_ptr<Connection> NodeGraphScene::createConnection(
//    PortType connectedPort,
//    Node& node,
//    PortIndex portIndex)
//{
//    auto connection = std::make_shared<Connection>(connectedPort, node, portIndex);

//    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *connection);

//    // after this function connection points are set to node port
//    connection->setGraphicsObject(std::move(cgo));

//    mConnections[connection->id()] = connection;

//    // Note: this connection isn't truly created yet. It's only partially created.
//    // Thus, don't send the connectionCreated(...) signal.

//    connect(connection.get(),
//            &Connection::connectionCompleted,
//            this,
//            [this](Connection const& c) {
//                connectionCreated(c);
//            });

//    return connection;
//}


//std::shared_ptr<Connection> NodeGraphScene::createConnection(
//    Node& nodeIn,
//    PortIndex portIndexIn,
//    Node& nodeOut,
//    PortIndex portIndexOut)
//{
//    auto connection = std::make_shared<Connection>(
//        nodeIn,
//        portIndexIn,
//        nodeOut,
//        portIndexOut);

//    auto cgo = std::make_unique<ConnectionGraphicsObject>(*this, *connection);

//    nodeIn.nodeState().setConnection(PortType::In, portIndexIn, *connection);
//    nodeOut.nodeState().setConnection(PortType::Out, portIndexOut, *connection);

//    // after this function connection points are set to node port
//    connection->setGraphicsObject(std::move(cgo));

//    // trigger data propagation
//    nodeOut.onDataUpdated(portIndexOut);

//    mConnections[connection->id()] = connection;

//    connectionCreated(*connection);

//    return connection;
//}


//std::shared_ptr<Connection> NodeGraphScene::restoreConnection(
//    QJsonObject const &connectionJson)
//{
//    QUuid nodeInId  = QUuid(connectionJson["in_id"].toString());
//    QUuid nodeOutId = QUuid(connectionJson["out_id"].toString());

//    PortIndex portIndexIn  = connectionJson["in_index"].toInt();
//    PortIndex portIndexOut = connectionJson["out_index"].toInt();

//    auto nodeIn  = mNodes[nodeInId].get();
//    auto nodeOut = mNodes[nodeOutId].get();

//    std::shared_ptr<Connection> connection =
//        createConnection(*nodeIn, portIndexIn,
//                         *nodeOut, portIndexOut);

//    // Note: the connectionCreated(...) signal has already been sent
//    // by createConnection(...)

//    return connection;
//}


//void NodeGraphScene::deleteConnection(Connection const& connection)
//{
//    auto it = mConnections.find(connection.id());
//    if (it != mConnections.end())
//    {
//        connection.removeFromNodes();
//        mConnections.erase(it);
//    }
//}


//Node& NodeGraphScene::createNode(std::unique_ptr<NodeDataModel>&& dataModel)
//{
//    auto node = detail::make_unique<Node>(std::move(dataModel));
//    auto ngo  = detail::make_unique<NodeGraphicsObject>(*this, *node);

//    node->setGraphicsObject(std::move(ngo));

//    auto nodePtr = node.get();
//    mNodes[node->id()] = std::move(node);

//    //emit nodeCreated(*nodePtr);
//    return *nodePtr;
//}


//Node& NodeGraphScene::restoreNode(QJsonObject const& nodeJson)
//{
//    QString modelName = nodeJson["model"].toObject()["name"].toString();

//    auto dataModel = registry().create(modelName);

//    if (!dataModel)
//        throw std::logic_error(std::string("No registered model with name ") +
//                               modelName.toLocal8Bit().data());

//    auto node = std::make_unique<Node>(std::move(dataModel));
//    auto ngo  = std::make_unique<NodeGraphicsObject>(*this, *node);
//    node->setGraphicsObject(std::move(ngo));

//    node->restore(nodeJson);

//    auto nodePtr = node.get();
//    mNodes[node->id()] = std::move(node);

//    emit nodePlaced(*nodePtr);
//    //nodeCreated(*nodePtr);
//    return *nodePtr;
//}


//void NodeGraphScene::removeNode(Node& node)
//{
//    // call signal
//    nodeDeleted(node);

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
//}


//DataModelRegistry& NodeGraphScene::registry() const
//{
//    return *mRegistry;
//}


//void NodeGraphScene::setRegistry(std::shared_ptr<DataModelRegistry> registry)
//{
//    mRegistry = std::move(registry);
//}


//void NodeGraphScene::iterateOverNodes(std::function<void(Node*)> const& visitor)
//{
//    for (const auto& _node : mNodes)
//    {
//        visitor(_node.second.get());
//    }
//}


//void NodeGraphScene::iterateOverNodeData(std::function<void(NodeDataModel*)> const& visitor)
//{
//    for (const auto& _node : mNodes)
//    {
//        visitor(_node.second->nodeDataModel());
//    }
//}


//void NodeGraphScene::iterateOverNodeDataDependentOrder(std::function<void(NodeDataModel*)> const& visitor)
//{
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
//}


//QPointF NodeGraphScene::getNodePosition(const Node& node) const
//{
//    return node.nodeGraphicsObject().pos();
//}


//void NodeGraphScene::setNodePosition(Node& node, const QPointF& pos) const
//{
//    node.nodeGraphicsObject().setPos(pos);
//    node.nodeGraphicsObject().moveConnections();
//}


QSizeF NodeGraphScene::getNodeSize(const Node& node) const
{
    return QSizeF(node.nodeGeometry().width(), node.nodeGeometry().height());
}


//std::unordered_map<QUuid, std::unique_ptr<Node> > const& NodeGraphScene::nodes() const
//{
//    return mNodes;
//}


//std::unordered_map<QUuid, std::shared_ptr<Connection> > const& NodeGraphScene::connections() const
//{
//    return mConnections;
//}





std::vector<Node*> NodeGraphScene::selectedNodes() const
{
    QList<QGraphicsItem*> graphicsItems = selectedItems();

    std::vector<Node*> ret;
    ret.reserve(graphicsItems.size());

    for (QGraphicsItem* item : graphicsItems)
    {
        auto ngo = qgraphicsitem_cast<NodeGraphicsObject*>(item);

        if (ngo != nullptr)
        {
            ret.push_back(&ngo->node());
        }
    }

    return ret;
}


//------------------------------------------------------------------------------

void NodeGraphScene::clearScene()
{
    // Manual node cleanup. Simply clearing the holding datastructures doesn't work, the code crashes when
    // there are both nodes and connections in the scene. (The data propagation internal logic tries to propagate
    // data through already freed connections.)
//    while (mConnections.size() > 0)
//    {
//        deleteConnection( *mConnections.begin()->second );
//    }

//    while (mNodes.size() > 0)
//    {
//        removeNode( *mNodes.begin()->second );
//    }
}


void NodeGraphScene::save() const
{
    QString fileName =
        QFileDialog::getSaveFileName(nullptr,
                                     tr("Open Flow Scene"),
                                     QDir::homePath(),
                                     tr("Flow Scene Files (*.flow)"));

    if (!fileName.isEmpty())
    {
        if (!fileName.endsWith("flow", Qt::CaseInsensitive))
            fileName += ".flow";

        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(saveToMemory());
        }
    }
}


void NodeGraphScene::load()
{
    QString fileName =
        QFileDialog::getOpenFileName(nullptr,
                                     tr("Open Flow Scene"),
                                     QDir::homePath(),
                                     tr("Flow Scene Files (*.flow)"));

    if (!QFileInfo::exists(fileName))
        return;

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    clearScene();

    QByteArray wholeFile = file.readAll();

    loadFromMemory(wholeFile);
}


QByteArray NodeGraphScene::saveToMemory() const
{
    QJsonObject sceneJson;

    QJsonArray nodesJsonArray;

    for (auto const & pair : mNodes)
    {
        auto const &node = pair.second;

        nodesJsonArray.append(node->save());
    }

    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray connectionJsonArray;
    for (auto const & pair : mConnections)
    {
        auto const &connection = pair.second;

        QJsonObject connectionJson = connection->save();

        if (!connectionJson.isEmpty())
            connectionJsonArray.append(connectionJson);
    }

    sceneJson["connections"] = connectionJsonArray;

    QJsonDocument document(sceneJson);

    return document.toJson();
}


void NodeGraphScene::loadFromMemory(const QByteArray& data)
{
//    QJsonObject const jsonDocument = QJsonDocument::fromJson(data).object();

//    QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

//    for (QJsonValueRef node : nodesJsonArray)
//    {
//        restoreNode(node.toObject());
//    }

//    QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

//    for (QJsonValueRef connection : connectionJsonArray)
//    {
//        restoreConnection(connection.toObject());
//    }
}


//void NodeGraphScene::setupConnectionSignals(Connection const& c)
//{
//    connect(&c, &Connection::connectionMadeIncomplete,
//            this, &NodeGraphScene::connectionDeleted, Qt::UniqueConnection);
//}


//void NodeGraphScene::sendConnectionCreatedToNodes(Connection const& c)
//{
//    Node* from = c.getNode(PortType::Out);
//    Node* to   = c.getNode(PortType::In);

//    Q_ASSERT(from != nullptr);
//    Q_ASSERT(to != nullptr);

//    from->nodeDataModel()->outputConnectionCreated(c);
//    to->nodeDataModel()->inputConnectionCreated(c);
//}


//void NodeGraphScene::sendConnectionDeletedToNodes(Connection const& c)
//{
//    Node* from = c.getNode(PortType::Out);
//    Node* to   = c.getNode(PortType::In);

//    Q_ASSERT(from != nullptr);
//    Q_ASSERT(to != nullptr);

//    from->nodeDataModel()->outputConnectionDeleted(c);
//    to->nodeDataModel()->inputConnectionDeleted(c);
//}


//------------------------------------------------------------------------------
namespace Cascade::NodeGraph
{

Node* locateNodeAt(
    QPointF scenePoint,
    NodeGraphScene &scene,
    QTransform const & viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items =
        scene.items(scenePoint,
                    Qt::IntersectsItemShape,
                    Qt::DescendingOrder,
                    viewTransform);

    //// items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem*> filteredItems;

    std::copy_if(items.begin(),
                 items.end(),
                 std::back_inserter(filteredItems),
                 [] (QGraphicsItem * item)
                 {
                     return (dynamic_cast<NodeGraphicsObject*>(item) != nullptr);
                 });

    Node* resultNode = nullptr;

    if (!filteredItems.empty())
    {
        QGraphicsItem* graphicsItem = filteredItems.front();
        auto ngo = dynamic_cast<NodeGraphicsObject*>(graphicsItem);

        resultNode = &ngo->node();
    }

    return resultNode;
}
}
