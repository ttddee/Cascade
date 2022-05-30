#include "nodeconnectioninteraction.h"

#include "connectiongraphicsobject.h"
#include "nodegraphicsobject.h"
#include "nodedatamodel.h"
#include "datamodelregistry.h"
#include "NodeGraphScene.h"

using Cascade::NodeGraph::NodeConnectionInteraction;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::NodeGraphScene;
using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::NodeDataModel;


NodeConnectionInteraction::NodeConnectionInteraction(
    Node& node, Connection& connection,
    NodeGraphScene& scene) :
    mNode(&node),
    mConnection(&connection),
    mScene(&scene)
{}


bool NodeConnectionInteraction::canConnect(
    PortIndex &portIndex) const
{
    // 1) Connection requires a port

    PortType requiredPort = connectionRequiredPort();


    if (requiredPort == PortType::None)
    {
        return false;
    }

    // 1.5) Forbid connecting the node to itself
    Node* node = mConnection->getNode(oppositePort(requiredPort));

    if (node == mNode)
        return false;

    // 2) connection point is on top of the node port

    QPointF connectionPoint = connectionEndScenePosition(requiredPort);

    portIndex = nodePortIndexUnderScenePoint(requiredPort,
                                             connectionPoint);

    if (portIndex == INVALID)
    {
        return false;
    }

    // 3) Node port is vacant

    // port should be empty
    if (!nodePortIsEmpty(requiredPort, portIndex))
        return false;

    // 4) Prevent loops in the graph
    // TODO: check if target node is in upstream

    return true;
}


bool NodeConnectionInteraction::tryConnect() const
{
    // 1) Check conditions from 'canConnect'
    PortIndex portIndex = INVALID;

    if (!canConnect(portIndex))
    {
        return false;
    }

    // 2) Assign node to required port in Connection
    PortType requiredPort = connectionRequiredPort();
    mNode->nodeState().setConnection(requiredPort,
                                     portIndex,
                                     *mConnection);

    // 3) Assign Connection to empty port in NodeState
    // The port is not longer required after this function
    mConnection->setNodeToPort(*mNode, requiredPort, portIndex);

    // 4) Adjust Connection geometry

    mNode->nodeGraphicsObject().moveConnections();

    // 5) Poke model to intiate data transfer

    auto outNode = mConnection->getNode(PortType::Out);
    if (outNode)
    {
        PortIndex outPortIndex = mConnection->getPortIndex(PortType::Out);
        outNode->onDataUpdated(outPortIndex);
    }

    return true;
}


/// 1) Node and Connection should be already connected
/// 2) If so, clear Connection entry in the NodeState
/// 3) Set Connection end to 'requiring a port'
bool NodeConnectionInteraction::disconnect(PortType portToDisconnect) const
{
    PortIndex portIndex =
        mConnection->getPortIndex(portToDisconnect);

    NodeState &state = mNode->nodeState();

    // clear pointer to Connection in the NodeState
    state.getEntries(portToDisconnect)[portIndex].erase(mConnection->id());

    // 4) Propagate invalid data to IN node
    mConnection->propagateEmptyData();

    // clear Connection side
    mConnection->clearNode(portToDisconnect);

    mConnection->setRequiredPort(portToDisconnect);

    mConnection->getConnectionGraphicsObject().grabMouse();

    return true;
}


// ------------------ util functions below

PortType NodeConnectionInteraction::connectionRequiredPort() const
{
    auto const &state = mConnection->connectionState();

    return state.requiredPort();
}


QPointF NodeConnectionInteraction::connectionEndScenePosition(PortType portType) const
{
    auto &go =
        mConnection->getConnectionGraphicsObject();

    ConnectionGeometry& geometry = mConnection->connectionGeometry();

    QPointF endPoint = geometry.getEndPoint(portType);

    return go.mapToScene(endPoint);
}


QPointF NodeConnectionInteraction::nodePortScenePosition(
    PortType portType,
    PortIndex portIndex) const
{
    NodeGeometry const &geom = mNode->nodeGeometry();

    QPointF p = geom.portScenePosition(portIndex, portType);

    NodeGraphicsObject& ngo = mNode->nodeGraphicsObject();

    return ngo.sceneTransform().map(p);
}


PortIndex NodeConnectionInteraction::nodePortIndexUnderScenePoint(
    PortType portType,
    QPointF const & scenePoint) const
{
    NodeGeometry const &nodeGeom = mNode->nodeGeometry();

    QTransform sceneTransform =
        mNode->nodeGraphicsObject().sceneTransform();

    PortIndex portIndex = nodeGeom.checkHitScenePoint(portType,
                                                      scenePoint,
                                                      sceneTransform);
    return portIndex;
}


bool NodeConnectionInteraction::nodePortIsEmpty(
    PortType portType,
    PortIndex portIndex) const
{
    NodeState const & nodeState = mNode->nodeState();

    auto const & entries = nodeState.getEntries(portType);
    auto const & connections = entries[portIndex];
    if (connections.empty()) return true;

    // Check if the connection already exists connected to the respective
    // input and output ports
    auto sourcePortType = oppositePort(portType);
    auto it = std::find_if(connections.begin(), connections.end(),
                           [this, sourcePortType](const auto& connection)
                           {
                               const auto* const currentConn = connection.second;

                               assert(mConnection->getNode(sourcePortType));
                               assert(currentConn->getNode(sourcePortType));
                               return mConnection->getNode(sourcePortType) == currentConn->getNode(sourcePortType) &&
                                      mConnection->getPortIndex(sourcePortType) == currentConn->getPortIndex(sourcePortType);
                           });
    if (it != connections.end())
        return false;

    switch (portType)
    {
    case PortType::In:
    {
        const auto policy = mNode->nodeDataModel()->portInConnectionPolicy(portIndex);
        return policy == NodeDataModel::ConnectionPolicy::Many;
    }
    case PortType::Out:
    {
        const auto policy = mNode->nodeDataModel()->portOutConnectionPolicy(portIndex);
        return policy == NodeDataModel::ConnectionPolicy::Many;
    }
    default: return false;
    }
}
