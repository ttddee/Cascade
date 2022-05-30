#include "nodestate.h"

#include "nodedatamodel.h"

#include "connection.h"

using Cascade::NodeGraph::NodeState;
using Cascade::NodeGraph::NodeDataType;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::Connection;

NodeState::NodeState(
    std::unique_ptr<NodeDataModel> const &model) :
    mInConnections(model->nPorts(PortType::In)),
    mOutConnections(model->nPorts(PortType::Out)),
    mReaction(NOT_REACTING),
    mReactingPortType(PortType::None),
    mResizing(false)
{}


std::vector<NodeState::ConnectionPtrSet> const& NodeState::getEntries(
    PortType portType) const
{
    if (portType == PortType::In)
        return mInConnections;
    else
        return mOutConnections;
}


std::vector<NodeState::ConnectionPtrSet>& NodeState::getEntries(
    PortType portType)
{
    if (portType == PortType::In)
        return mInConnections;
    else
        return mOutConnections;
}


NodeState::ConnectionPtrSet NodeState::connections(
    PortType portType,
    PortIndex portIndex) const
{
    auto const &connections = getEntries(portType);

    return connections[portIndex];
}


void NodeState::setConnection(
    PortType portType,
    PortIndex portIndex,
    Connection& connection)
{
    auto &connections = getEntries(portType);

    connections.at(portIndex).insert(std::make_pair(connection.id(),
                                                    &connection));
}


void NodeState::eraseConnection(
    PortType portType,
    PortIndex portIndex,
    QUuid id)
{
    getEntries(portType)[portIndex].erase(id);
}


NodeState::ReactToConnectionState NodeState::reaction() const
{
    return mReaction;
}


PortType NodeState::reactingPortType() const
{
    return mReactingPortType;
}


NodeDataType NodeState::reactingDataType() const
{
    return mReactingDataType;
}


void NodeState::setReaction(
    ReactToConnectionState reaction,
    PortType reactingPortType,
    NodeDataType reactingDataType)
{
    mReaction = reaction;

    mReactingPortType = reactingPortType;

    mReactingDataType = std::move(reactingDataType);
}


bool NodeState::isReacting() const
{
    return mReaction == REACTING;
}


void NodeState::setResizing(bool resizing)
{
    mResizing = resizing;
}


bool NodeState::resizing() const
{
    return mResizing;
}
