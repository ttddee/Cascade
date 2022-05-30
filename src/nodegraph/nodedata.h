#pragma once

#include <vector>

#include <QtCore/QString>

namespace Cascade::NodeGraph
{

struct NodeDataType
{
    QString id;
    QString name;

    friend bool operator<(
        Cascade::NodeGraph::NodeDataType const& d1,
        Cascade::NodeGraph::NodeDataType const& d2)
    {
        return d1.id < d2.id;
    }

    friend bool operator==(
        const Cascade::NodeGraph::NodeDataType& d1,
        const Cascade::NodeGraph::NodeDataType& d2) noexcept
    {
        return d1.id == d2.id;
    }
};

/// Class represents data transferred between nodes.
/// @param type is used for comparing the types
/// The actual data is stored in subtypes
struct NodeData
{
//    virtual ~NodeData() = default;

//    virtual bool sameType(NodeData const &nodeData) const
//    {
//        return (this->type().id == nodeData.type().id);
//    }

//    /// Type for inner use
//    virtual NodeDataType type() const { return NodeDataType{"te", "st"}; };

    QString mCaption;
    QString mName;
    std::vector<QString> mInPorts;
    std::vector<QString> mOutPorts;

};
}
