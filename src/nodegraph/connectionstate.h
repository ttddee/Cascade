#pragma once

#include <QtCore/QUuid>

#include "porttype.h"

class QPointF;

namespace Cascade::NodeGraph
{

class Node;

/// Stores currently draggind end.
/// Remembers last hovered Node.
class ConnectionState
{
public:
    ConnectionState(PortType port = PortType::None)
        : mRequiredPort(port)
    {}

    ConnectionState(const ConnectionState&) = delete;
    ConnectionState operator=(const ConnectionState&) = delete;

    ~ConnectionState();

public:
    void setRequiredPort(PortType end)
    { mRequiredPort = end; }

    PortType requiredPort() const
    { return mRequiredPort; }

    bool requiresPort() const
    { return mRequiredPort != PortType::None; }

    void setNoRequiredPort()
    { mRequiredPort = PortType::None; }

public:
    void interactWithNode(Node* node);

    void setLastHoveredNode(Node* node);

    Node* lastHoveredNode() const
    { return mLastHoveredNode; }

    void resetLastHoveredNode();

private:
    PortType mRequiredPort;

    Node* mLastHoveredNode { nullptr };
};
}
