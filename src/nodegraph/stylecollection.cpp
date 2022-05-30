#include "stylecollection.h"

using Cascade::NodeGraph::StyleCollection;
using Cascade::NodeGraph::NodeStyle;
using Cascade::NodeGraph::ConnectionStyle;
using Cascade::NodeGraph::NodeGraphViewStyle;

NodeStyle const& StyleCollection::nodeStyle()
{
    return instance().mNodeStyle;
}


ConnectionStyle const& StyleCollection::connectionStyle()
{
    return instance().mConnectionStyle;
}


NodeGraphViewStyle const& StyleCollection::nodeGraphViewStyle()
{
    return instance().mNodeGraphViewStyle;
}


void StyleCollection::setNodeStyle(NodeStyle nodeStyle)
{
    instance().mNodeStyle = nodeStyle;
}


void StyleCollection::setConnectionStyle(ConnectionStyle connectionStyle)
{
    instance().mConnectionStyle = connectionStyle;
}


void StyleCollection::setnodeGraphViewStyle(NodeGraphViewStyle nodeGraphViewStyle)
{
    instance().mNodeGraphViewStyle = nodeGraphViewStyle;
}


StyleCollection& StyleCollection::instance()
{
    static StyleCollection collection;

    return collection;
}
