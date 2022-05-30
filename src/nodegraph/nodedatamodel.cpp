#include "nodedatamodel.h"

#include "stylecollection.h"

using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeStyle;
using Cascade::NodeGraph::NodeData;

NodeDataModel::NodeDataModel() :
    mNodeStyle(StyleCollection::nodeStyle())
{
    // Derived classes can initialize specific style here
}


QJsonObject NodeDataModel::save() const
{
    QJsonObject modelJson;

    //modelJson["name"] = name();

    return modelJson;
}


NodeStyle const& NodeDataModel::nodeStyle() const
{
    return mNodeStyle;
}


void NodeDataModel::setNodeStyle(NodeStyle const& style)
{
    mNodeStyle = style;
}
