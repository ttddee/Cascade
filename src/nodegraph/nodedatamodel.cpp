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
