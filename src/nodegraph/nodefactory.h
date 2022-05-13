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
*/

#ifndef NODEFACTORY_H
#define NODEFACTORY_H

#include "nodebase.h"
#include "readnode.h"
#include "isfnode.h"
#include "cropnode.h"
#include "resizenode.h"

namespace Cascade {

class NodeFactory
{
public:
    static NodeBase* createNode(
            const NodeType type,
            NodeGraph* graph,
            QWidget *parent,
            const QString& customName)
    {
        if (type == NodeType::eRead)
        {
            return new ReadNode(type, graph, parent, customName);
        }
        else if (type == NodeType::eCrop)
        {
            return new CropNode(type, graph, parent, customName);
        }
        else if (type == NodeType::eResize)
        {
            return new ResizeNode(type, graph, parent, customName);
        }
        else if (type == NodeType::eIsf)
        {
            return new IsfNode(type, graph, parent, customName);
        }
        else
        {
            return new NodeBase(type, graph, parent, customName);
        }
    }
};

} // namespace Cascade

#endif // NODEFACTORY_H
