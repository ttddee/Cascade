/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#pragma once

#include <vector>

#include <QtCore/QString>

#include "propertymodel.h"

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
    std::vector<std::unique_ptr<PropertyModel>> mProperties;

};
}
