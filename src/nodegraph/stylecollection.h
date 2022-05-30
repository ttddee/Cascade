#pragma once

#include "nodestyle.h"
#include "connectionstyle.h"
#include "nodegraphviewstyle.h"

namespace Cascade::NodeGraph
{

class StyleCollection
{
public:
    static NodeStyle const&  nodeStyle();

    static ConnectionStyle const& connectionStyle();

    static NodeGraphViewStyle const& nodeGraphViewStyle();

public:
    static void setNodeStyle(NodeStyle);

    static void setConnectionStyle(ConnectionStyle);

    static void setnodeGraphViewStyle(NodeGraphViewStyle);

private:
    StyleCollection() = default;

    StyleCollection(StyleCollection const&) = delete;

    StyleCollection& operator=(StyleCollection const&) = delete;

    static StyleCollection& instance();

private:
    NodeStyle mNodeStyle;

    ConnectionStyle mConnectionStyle;

    NodeGraphViewStyle mNodeGraphViewStyle;
};
}
