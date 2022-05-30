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
