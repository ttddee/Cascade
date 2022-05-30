#include "connectionstate.h"

#include <iostream>

#include <QtCore/QPointF>

#include "NodeGraphScene.h"
#include "node.h"

using Cascade::NodeGraph::ConnectionState;
using Cascade::NodeGraph::Node;

ConnectionState::~ConnectionState()
{
    resetLastHoveredNode();
}


void ConnectionState::interactWithNode(Node* node)
{
    if (node)
    {
        mLastHoveredNode = node;
    }
    else
    {
        resetLastHoveredNode();
    }
}


void ConnectionState::setLastHoveredNode(Node* node)
{
    mLastHoveredNode = node;
}


void ConnectionState::resetLastHoveredNode()
{
    if (mLastHoveredNode)
        mLastHoveredNode->resetReactionToConnection();

    mLastHoveredNode = nullptr;
}
