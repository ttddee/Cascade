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

#include "node.h"

#include <QtCore/QObject>

#include <utility>
#include <iostream>

#include "nodegraphscene.h"

#include "nodegraphicsobject.h"
#include "nodedatamodel.h"

#include "connectiongraphicsobject.h"
#include "connectionstate.h"

using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeGeometry;
using Cascade::NodeGraph::NodeState;
using Cascade::NodeGraph::NodeData;
using Cascade::NodeGraph::NodeDataType;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeGraphicsObject;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::PortType;

Node::
    Node(std::unique_ptr<NodeDataModel> && dataModel) :
    mUid(QUuid::createUuid()),
    mNodeDataModel(std::move(dataModel)),
    mNodeState(mNodeDataModel),
    mNodeGeometry(mNodeDataModel),
    mNodeGraphicsObject(nullptr)
{
    mNodeGeometry.recalculateSize();

    // propagate data: model => node
    connect(mNodeDataModel.get(), &NodeDataModel::dataUpdated,
            this, &Node::onDataUpdated);

//    connect(mNodeDataModel.get(), &NodeDataModel::embeddedWidgetSizeUpdated,
//            this, &Node::onNodeSizeUpdated );
}


Node::~Node() = default;

QJsonObject Node::save() const
{
    QJsonObject nodeJson;

    nodeJson["id"] = mUid.toString();

    nodeJson["model"] = mNodeDataModel->save();

    QJsonObject obj;
    obj["x"] = mNodeGraphicsObject->pos().x();
    obj["y"] = mNodeGraphicsObject->pos().y();
    nodeJson["position"] = obj;

    return nodeJson;
}


void Node::restore(QJsonObject const& json)
{
    mUid = QUuid(json["id"].toString());

    QJsonObject positionJson = json["position"].toObject();
    QPointF     point(positionJson["x"].toDouble(),
                  positionJson["y"].toDouble());
    mNodeGraphicsObject->setPos(point);

    mNodeDataModel->restore(json["model"].toObject());
}


QUuid Node::id() const
{
    return mUid;
}


void Node::reactToPossibleConnection(PortType reactingPortType,
                                     NodeDataType const &reactingDataType,
                                     QPointF const &scenePoint)
{
    QTransform const t = mNodeGraphicsObject->sceneTransform();

    QPointF p = t.inverted().map(scenePoint);

    mNodeGeometry.setDraggingPosition(p);

    mNodeGraphicsObject->update();

    mNodeState.setReaction(NodeState::REACTING,
                           reactingPortType,
                           reactingDataType);
}


void Node::resetReactionToConnection()
{
    mNodeState.setReaction(NodeState::NOT_REACTING);
    mNodeGraphicsObject->update();
}


NodeGraphicsObject const& Node::nodeGraphicsObject() const
{
    return *mNodeGraphicsObject.get();
}


NodeGraphicsObject& Node::nodeGraphicsObject()
{
    return *mNodeGraphicsObject.get();
}


void Node::setGraphicsObject(std::unique_ptr<NodeGraphicsObject>&& graphics)
{
    mNodeGraphicsObject = std::move(graphics);

    mNodeGeometry.recalculateSize();
}


NodeGeometry& Node::nodeGeometry()
{
    return mNodeGeometry;
}


NodeGeometry const& Node::nodeGeometry() const
{
    return mNodeGeometry;
}


NodeState const& Node::nodeState() const
{
    return mNodeState;
}


NodeState& Node::nodeState()
{
    return mNodeState;
}


NodeDataModel* Node::nodeDataModel() const
{
    return mNodeDataModel.get();
}


PropertyWidget* Node::propertyWidget()
{
    if (!mPropertyWidget)
    {
        mPropertyWidget = new PropertyWidget();
        mPropertyWidget->addPropertyViews(mNodeDataModel->getPropertyViews());
    }
    return mPropertyWidget;
}


void Node::propagateData(
    std::shared_ptr<NodeData> nodeData,
    PortIndex inPortIndex,
    const QUuid& connectionId) const
{
    mNodeDataModel->setInData(std::move(nodeData), inPortIndex, connectionId);

    //Recalculate the nodes visuals. A data change can result in the node taking more space than before, so this forces a recalculate+repaint on the affected node
    mNodeGraphicsObject->setGeometryChanged();
    mNodeGeometry.recalculateSize();
    mNodeGraphicsObject->update();
    mNodeGraphicsObject->moveConnections();
}


void Node::onDataUpdated(PortIndex index)
{
    auto nodeData = mNodeDataModel->outData(index);

    auto connections =
        mNodeState.connections(PortType::Out, index);

    for (auto const & c : connections)
        c.second->propagateData(nodeData);
}

void Node::onNodeSizeUpdated()
{
//    if( nodeDataModel()->embeddedWidget() )
//    {
//        nodeDataModel()->embeddedWidget()->adjustSize();
//    }
    nodeGeometry().recalculateSize();
    for(PortType type: {PortType::In, PortType::Out})
    {
        for(auto& conn_set : nodeState().getEntries(type))
        {
            for(auto& pair: conn_set)
            {
                Connection* conn = pair.second;
                conn->getConnectionGraphicsObject().move();
            }
        }
    }
}
