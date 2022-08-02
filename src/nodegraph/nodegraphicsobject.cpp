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

#include "nodegraphicsobject.h"

#include <iostream>
#include <cstdlib>

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QGraphicsEffect>

#include "connectiongraphicsobject.h"
#include "connectionstate.h"

#include "nodegraphdatamodel.h"
#include "nodegraphscene.h"
#include "nodepainter.h"

#include "node.h"
#include "nodedatamodel.h"
#include "nodeconnectioninteraction.h"

#include "stylecollection.h"

using Cascade::NodeGraph::NodeGraphicsObject;
using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeGraphScene;

NodeGraphicsObject::NodeGraphicsObject(
    NodeGraphDataModel &model,
    NodeGraphScene &scene,
    Node& node) :
    mModel(model),
    mScene(scene),
    mNode(node),
    mLocked(false),
    mProxyWidget(nullptr)
{
    mScene.addItem(this);

    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    auto const &nodeStyle = node.nodeDataModel()->nodeStyle();

    {
        auto effect = new QGraphicsDropShadowEffect;
        effect->setOffset(4, 4);
        effect->setBlurRadius(20);
        effect->setColor(nodeStyle.ShadowColor);

        setGraphicsEffect(effect);
    }

    setOpacity(nodeStyle.Opacity);

    setAcceptHoverEvents(true);

    setZValue(0);

    // connect to the move signals to emit the move signals in FlowScene
    auto onMoveSlot = [this] {
        emit mScene.nodeMoved(mNode, pos());
    };
    connect(this, &QGraphicsObject::xChanged, this, onMoveSlot);
    connect(this, &QGraphicsObject::yChanged, this, onMoveSlot);
}


NodeGraphicsObject::~NodeGraphicsObject()
{
    mScene.removeItem(this);
}


Node& NodeGraphicsObject::node()
{
    return mNode;
}


Node const& NodeGraphicsObject::node() const
{
    return mNode;
}


QRectF NodeGraphicsObject::boundingRect() const
{
    return mNode.nodeGeometry().boundingRect();
}


void NodeGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}


void NodeGraphicsObject::moveConnections() const
{
    NodeState const & nodeState = mNode.nodeState();

    for (PortType portType: {PortType::In, PortType::Out})
    {
        auto const & connectionEntries =
            nodeState.getEntries(portType);

        for (auto const & connections : connectionEntries)
        {
            for (auto & con : connections)
                con.second->getConnectionGraphicsObject().move();
        }
    }
}


void NodeGraphicsObject::lock(bool locked)
{
    mLocked = locked;

    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}


void NodeGraphicsObject::paint(
    QPainter * painter,
    QStyleOptionGraphicsItem const* option,
    QWidget* )
{
    painter->setClipRect(option->exposedRect);

    NodePainter::paint(painter, mNode, mScene);
}


QVariant NodeGraphicsObject::itemChange(
    GraphicsItemChange change,
    const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        moveConnections();
    }

    return QGraphicsItem::itemChange(change, value);
}


void NodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    if (mLocked)
        return;

    // deselect all other items after this one is selected
    if (!isSelected() &&
        !(event->modifiers() & Qt::ControlModifier))
    {
        mScene.clearSelection();
    }

    for (PortType portToCheck: {PortType::In, PortType::Out})
    {
        NodeGeometry const & nodeGeometry = mNode.nodeGeometry();

        // TODO do not pass sceneTransform
        int const portIndex = nodeGeometry.checkHitScenePoint(portToCheck,
                                                              event->scenePos(),
                                                              sceneTransform());

        if (portIndex != INVALID)
        {
            NodeState const & nodeState = mNode.nodeState();

            std::unordered_map<QUuid, Connection*> connections =
                nodeState.connections(portToCheck, portIndex);

            // start dragging existing connection
            if (!connections.empty() && portToCheck == PortType::In)
            {
                auto con = connections.begin()->second;

                NodeConnectionInteraction interaction(mNode, *con, mScene);

                interaction.disconnect(portToCheck);
            }
            else // initialize new Connection
            {
                if (portToCheck == PortType::Out)
                {
                    auto const outPolicy = mNode.nodeDataModel()->portOutConnectionPolicy(portIndex);
                    if (!connections.empty() &&
                        outPolicy == NodeDataModel::ConnectionPolicy::One)
                    {
                        mModel.deleteConnection( *connections.begin()->second );
                    }
                }

                // todo add to FlowScene
                auto connection = mModel.createConnection(portToCheck,
                                                          mNode,
                                                          portIndex);

                mNode.nodeState().setConnection(portToCheck,
                                                portIndex,
                                                *connection);

                connection->getConnectionGraphicsObject().grabMouse();
            }
        }
    }

    auto pos     = event->pos();
    auto & geom  = mNode.nodeGeometry();
    auto & state = mNode.nodeState();

    if (mNode.nodeDataModel()->resizable() &&
        geom.resizeRect().contains(QPoint(pos.x(),
                                          pos.y())))
    {
        state.setResizing(true);
    }
}


void NodeGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
     [[maybe_unused]] auto & geom  = mNode.nodeGeometry();
    auto & state = mNode.nodeState();

    if (state.resizing())
    {
         [[maybe_unused]] auto diff = event->pos() - event->lastPos();

//        if (auto w = mNode.nodeDataModel()->embeddedWidget())
//        {
//            prepareGeometryChange();

//            auto oldSize = w->size();

//            oldSize += QSize(diff.x(), diff.y());

//            w->setFixedSize(oldSize);

//            mProxyWidget->setMinimumSize(oldSize);
//            mProxyWidget->setMaximumSize(oldSize);
//            mProxyWidget->setPos(geom.widgetPosition());

//            geom.recalculateSize();
//            update();

//            moveConnections();

//            event->accept();
//        }
    }
    else
    {
        QGraphicsObject::mouseMoveEvent(event);

        if (event->lastPos() != event->pos())
            moveConnections();

        event->ignore();
    }

    QRectF r = scene()->sceneRect();

    r = r.united(mapToScene(boundingRect()).boundingRect());

    scene()->setSceneRect(r);
}


void NodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    auto & state = mNode.nodeState();

    state.setResizing(false);

    QGraphicsObject::mouseReleaseEvent(event);

    // position connections precisely after fast node move
    moveConnections();

    emit mScene.nodeClicked(node());
}


void NodeGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    // bring all the colliding nodes to background
    QList<QGraphicsItem *> overlapItems = collidingItems();

    for (QGraphicsItem *item : overlapItems)
    {
        if (item->zValue() > 0.0)
        {
            item->setZValue(0.0);
        }
    }

    // bring this node forward
    setZValue(1.0);

    mNode.nodeGeometry().setHovered(true);
    update();
    emit mScene.nodeHovered(node(), event->screenPos());
    event->accept();
}


void NodeGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    mNode.nodeGeometry().setHovered(false);
    update();
    emit mScene.nodeHoverLeft(node());
    event->accept();
}


void NodeGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    auto pos    = event->pos();
    auto & geom = mNode.nodeGeometry();

    if (mNode.nodeDataModel()->resizable() &&
        geom.resizeRect().contains(QPoint(pos.x(), pos.y())))
    {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else
    {
        setCursor(QCursor());
    }

    event->accept();
}


void NodeGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    emit mScene.nodeDoubleClicked(&node());
}


void NodeGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    emit mScene.nodeContextMenu(node(), mapToScene(event->pos()));
}
