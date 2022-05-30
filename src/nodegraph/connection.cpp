#include "connection.h"

#include <cmath>
#include <utility>

#include <QtWidgets/QtWidgets>
#include <QtGlobal>

#include "node.h"
#include "NodeGraphScene.h"
#include "nodegraphview.h"

#include "nodegeometry.h"
#include "nodegraphicsobject.h"
#include "nodedatamodel.h"

#include "connectionstate.h"
#include "connectiongeometry.h"
#include "connectiongraphicsobject.h"

using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::ConnectionState;
using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeData;
using Cascade::NodeGraph::NodeDataType;
using Cascade::NodeGraph::ConnectionGraphicsObject;
using Cascade::NodeGraph::ConnectionGeometry;

Connection::Connection(
    PortType portType,
    Node& node,
    PortIndex portIndex) :
    mUid(QUuid::createUuid()),
    mOutPortIndex(INVALID),
    mInPortIndex(INVALID),
    mConnectionState()
{
    setNodeToPort(node, portType, portIndex);

    setRequiredPort(oppositePort(portType));
}


Connection:: Connection(
    Node& nodeIn,
    PortIndex portIndexIn,
    Node& nodeOut,
    PortIndex portIndexOut) :
    mUid(QUuid::createUuid()),
    mOutNode(&nodeOut),
    mInNode(&nodeIn),
    mOutPortIndex(portIndexOut),
    mInPortIndex(portIndexIn),
    mConnectionState()
{
    setNodeToPort(nodeIn, PortType::In, portIndexIn);
    setNodeToPort(nodeOut, PortType::Out, portIndexOut);
}

Connection::~Connection()
{
    if (isComplete())
    {
        emit connectionMadeIncomplete(*this);
    }

    if (mInNode)
    {
        mInNode->nodeGraphicsObject().update();
    }

    if (mOutNode)
    {
        propagateEmptyData();
        mOutNode->nodeGraphicsObject().update();
    }
}


QJsonObject Connection::save() const
{
    QJsonObject connectionJson;

    if (mInNode && mOutNode)
    {
        connectionJson["in_id"] = mInNode->id().toString();
        connectionJson["in_index"] = mInPortIndex;

        connectionJson["out_id"] = mOutNode->id().toString();
        connectionJson["out_index"] = mOutPortIndex;
    }

    return connectionJson;
}


QUuid Connection:: id() const
{
    return mUid;
}


bool Connection::isComplete() const
{
    return mInNode != nullptr && mOutNode != nullptr;
}


void Connection::setRequiredPort(PortType dragging)
{
    mConnectionState.setRequiredPort(dragging);

    switch (dragging)
    {
    case PortType::Out:
        mOutNode      = nullptr;
        mOutPortIndex = INVALID;
        break;

    case PortType::In:
        mInNode      = nullptr;
        mInPortIndex = INVALID;
        break;

    default:
        break;
    }
}


PortType Connection::requiredPort() const
{
    return mConnectionState.requiredPort();
}


void Connection::setGraphicsObject(
    std::unique_ptr<ConnectionGraphicsObject>&& graphics)
{
    mConnectionGraphicsObject = std::move(graphics);

    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in Connection G.O. coordinates. The position of the whole
    // Connection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (requiredPort() != PortType::None)
    {

        PortType attachedPort = oppositePort(requiredPort());

        PortIndex attachedPortIndex = getPortIndex(attachedPort);

        auto node = getNode(attachedPort);

        QTransform nodeSceneTransform =
            node->nodeGraphicsObject().sceneTransform();

        QPointF pos = node->nodeGeometry().portScenePosition(attachedPortIndex,
                                                             attachedPort,
                                                             nodeSceneTransform);

        mConnectionGraphicsObject->setPos(pos);
    }

    mConnectionGraphicsObject->move();
}



PortIndex Connection::getPortIndex(PortType portType) const
{
    PortIndex result = INVALID;

    switch (portType)
    {
    case PortType::In:
        result = mInPortIndex;
        break;

    case PortType::Out:
        result = mOutPortIndex;

        break;

    default:
        break;
    }

    return result;
}


void Connection::setNodeToPort(
    Node& node,
    PortType portType,
    PortIndex portIndex)
{
    bool wasIncomplete = !isComplete();

    auto& nodeWeak = getNode(portType);

    nodeWeak = &node;

    if (portType == PortType::Out)
        mOutPortIndex = portIndex;
    else
        mInPortIndex = portIndex;

    mConnectionState.setNoRequiredPort();

    emit updated(*this);
    if (isComplete() && wasIncomplete) {
        emit connectionCompleted(*this);
    }
}


void Connection::removeFromNodes() const
{
    if (mInNode)
        mInNode->nodeState().eraseConnection(PortType::In, mInPortIndex, id());

    if (mOutNode)
        mOutNode->nodeState().eraseConnection(PortType::Out, mOutPortIndex, id());
}


ConnectionGraphicsObject& Connection::getConnectionGraphicsObject() const
{
    return *mConnectionGraphicsObject;
}


ConnectionState& Connection::connectionState()
{
    return mConnectionState;
}


ConnectionState const& Connection::connectionState() const
{
    return mConnectionState;
}


ConnectionGeometry& Connection::connectionGeometry()
{
    return mConnectionGeometry;
}


ConnectionGeometry const& Connection::connectionGeometry() const
{
    return mConnectionGeometry;
}


Node* Connection::getNode(PortType portType) const
{
    switch (portType)
    {
    case PortType::In:
        return mInNode;
        break;

    case PortType::Out:
        return mOutNode;
        break;

    default:
        // not possible
        break;
    }
    return nullptr;
}


Node*& Connection::getNode(PortType portType)
{
    switch (portType)
    {
    case PortType::In:
        return mInNode;
        break;

    case PortType::Out:
        return mOutNode;
        break;

    default:
        // not possible
        break;
    }
    Q_UNREACHABLE();
}


void Connection::clearNode(PortType portType)
{
    if (isComplete())
    {
        emit connectionMadeIncomplete(*this);
    }

    getNode(portType) = nullptr;

    if (portType == PortType::In)
        mInPortIndex = INVALID;
    else
        mOutPortIndex = INVALID;
}


NodeDataType Connection::dataType(PortType portType) const
{
    if (mInNode && mOutNode)
    {
        auto const & model = (portType == PortType::In) ?
                                mInNode->nodeDataModel() :
                                mOutNode->nodeDataModel();
        PortIndex index = (portType == PortType::In) ?
                              mInPortIndex :
                              mOutPortIndex;

        return model->dataType(portType, index);
    }
    else
    {
        Node* validNode;
        PortIndex index = INVALID;

        if ((validNode = mInNode))
        {
            index    = mInPortIndex;
            portType = PortType::In;
        }
        else if ((validNode = mOutNode))
        {
            index    = mOutPortIndex;
            portType = PortType::Out;
        }

        if (validNode)
        {
            auto const &model = validNode->nodeDataModel();

            return model->dataType(portType, index);
        }
    }

    Q_UNREACHABLE();
}


void Connection::propagateData(std::shared_ptr<NodeData> nodeData) const
{
    if (mInNode)
    {
        mInNode->propagateData(nodeData, mInPortIndex, id());
    }
}


void Connection::propagateEmptyData() const
{
    std::shared_ptr<NodeData> emptyData;

    propagateData(emptyData);
}
