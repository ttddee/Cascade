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

#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsSceneMoveEvent>

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QtGlobal>

#include "node.h"
#include "nodegraphicsobject.h"

#include "connectiongraphicsobject.h"
#include "nodegraphicsobject.h"

#include "connection.h"

#include "datamodelregistry.h"
#include "nodegraphview.h"

using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::DataModelRegistry;
using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeGraphDataModel;
using Cascade::NodeGraph::NodeGraphicsObject;
using Cascade::NodeGraph::NodeGraphScene;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::PortType;

NodeGraphScene::NodeGraphScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

NodeGraphScene::~NodeGraphScene()
{
    //clearScene();
}

//------------------------------------------------------------------------------

QSizeF NodeGraphScene::getNodeSize(const Node& node) const
{
    return QSizeF(node.nodeGeometry().width(), node.nodeGeometry().height());
}

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
    QString fileName = QFileDialog::getSaveFileName(
        nullptr, tr("Open Flow Scene"), QDir::homePath(), tr("Flow Scene Files (*.flow)"));

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
    QString fileName = QFileDialog::getOpenFileName(
        nullptr, tr("Open Flow Scene"), QDir::homePath(), tr("Flow Scene Files (*.flow)"));

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

    for (auto const& pair : mNodes)
    {
        auto const& node = pair.second;

        nodesJsonArray.append(node->save());
    }

    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray connectionJsonArray;
    for (auto const& pair : mConnections)
    {
        auto const& connection = pair.second;

        QJsonObject connectionJson = connection->save();

        if (!connectionJson.isEmpty())
            connectionJsonArray.append(connectionJson);
    }

    sceneJson["connections"] = connectionJsonArray;

    QJsonDocument document(sceneJson);

    return document.toJson();
}

void NodeGraphScene::loadFromMemory( [[maybe_unused]] const QByteArray& data)
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

Node* locateNodeAt(QPointF scenePoint, NodeGraphScene& scene, QTransform const& viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items =
        scene.items(scenePoint, Qt::IntersectsItemShape, Qt::DescendingOrder, viewTransform);

    //// items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem*> filteredItems;

    std::copy_if(
        items.begin(), items.end(), std::back_inserter(filteredItems), [](QGraphicsItem* item) {
            return (dynamic_cast<NodeGraphicsObject*>(item) != nullptr);
        });

    Node* resultNode = nullptr;

    if (!filteredItems.empty())
    {
        QGraphicsItem* graphicsItem = filteredItems.front();
        auto ngo                    = dynamic_cast<NodeGraphicsObject*>(graphicsItem);

        resultNode = &ngo->node();
    }

    return resultNode;
}
} // namespace Cascade::NodeGraph
