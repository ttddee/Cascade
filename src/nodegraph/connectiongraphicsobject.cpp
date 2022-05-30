#include "connectiongraphicsobject.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtWidgets/QGraphicsBlurEffect>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QGraphicsView>

#include "NodeGraphScene.h"

#include "connection.h"
#include "connectiongeometry.h"
#include "connectionpainter.h"
#include "connectionstate.h"

#include "nodegraphicsobject.h"

#include "nodeconnectioninteraction.h"

#include "node.h"

using Cascade::NodeGraph::ConnectionGraphicsObject;
using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::NodeGraphScene;

ConnectionGraphicsObject::ConnectionGraphicsObject(
    NodeGraphScene &scene,
    Connection &connection) :
    mScene(scene),
    mConnection(connection)
{
    mScene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    // addGraphicsEffect();

    setZValue(-1.0);
}


ConnectionGraphicsObject::~ConnectionGraphicsObject()
{
    mScene.removeItem(this);
}


Cascade::NodeGraph::Connection& ConnectionGraphicsObject::connection()
{
    return mConnection;
}


QRectF ConnectionGraphicsObject::boundingRect() const
{
    return mConnection.connectionGeometry().boundingRect();
}


QPainterPath ConnectionGraphicsObject::shape() const
{
#ifdef DEBUG_DRAWING

//QPainterPath path;

//path.addRect(boundingRect());
//return path;

#else
    auto const &geom =
        mConnection.connectionGeometry();

    return ConnectionPainter::getPainterStroke(geom);

#endif
}


void ConnectionGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}


void ConnectionGraphicsObject::move()
{
    for(PortType portType: { PortType::In, PortType::Out } )
    {
        if (auto node = mConnection.getNode(portType))
        {
            auto const &nodeGraphics = node->nodeGraphicsObject();

            auto const &nodeGeom = node->nodeGeometry();

            QPointF scenePos =
                nodeGeom.portScenePosition(mConnection.getPortIndex(portType),
                                           portType,
                                           nodeGraphics.sceneTransform());

            QTransform sceneTransform = this->sceneTransform();

            QPointF connectionPos = sceneTransform.inverted().map(scenePos);

            mConnection.connectionGeometry().setEndPoint(portType,
                                                         connectionPos);

            mConnection.getConnectionGraphicsObject().setGeometryChanged();
            mConnection.getConnectionGraphicsObject().update();
        }
    }

}

void ConnectionGraphicsObject::lock(bool locked)
{
    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsFocusable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
}


void ConnectionGraphicsObject::paint(
    QPainter* painter,
    QStyleOptionGraphicsItem const* option,
    QWidget*)
{
    painter->setClipRect(option->exposedRect);

    ConnectionPainter::paint(painter,
                             mConnection);
}


void ConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
    //event->ignore();
}


void ConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    prepareGeometryChange();

    auto view = static_cast<QGraphicsView*>(event->widget());
    auto node = locateNodeAt(event->scenePos(),
                             mScene,
                             view->transform());

    auto &state = mConnection.connectionState();

    state.interactWithNode(node);
    if (node)
    {
        node->reactToPossibleConnection(state.requiredPort(),
                                        mConnection.dataType(oppositePort(state.requiredPort())),
                                        event->scenePos());
    }

    //-------------------

    QPointF offset = event->pos() - event->lastPos();

    auto requiredPort = mConnection.requiredPort();

    if (requiredPort != PortType::None)
    {
        mConnection.connectionGeometry().moveEndPoint(requiredPort, offset);
    }

    //-------------------

    update();

    event->accept();
}


void ConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    ungrabMouse();
    event->accept();

    auto node = locateNodeAt(event->scenePos(), mScene,
                             mScene.views()[0]->transform());

    NodeConnectionInteraction interaction(*node, mConnection, mScene);

    if (node && interaction.tryConnect())
    {
        node->resetReactionToConnection();
    }

    if (mConnection.connectionState().requiresPort())
    {
        mScene.deleteConnection(mConnection);
    }
}


void ConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    mConnection.connectionGeometry().setHovered(true);

    update();
    mScene.connectionHovered(connection(), event->screenPos());
    event->accept();
}


void ConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    mConnection.connectionGeometry().setHovered(false);

    update();
    mScene.connectionHoverLeft(connection());
    event->accept();
}


void ConnectionGraphicsObject::addGraphicsEffect()
{
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    //auto effect = new QGraphicsDropShadowEffect;
    //auto effect = new ConnectionBlurEffect(this);
    //effect->setOffset(4, 4);
    //effect->setColor(QColor(Qt::gray).darker(800));
}
