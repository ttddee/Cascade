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

#include "cropnode.h"

#include "log.h"

namespace Cascade {

CropNode::CropNode(const NodeType type,
                   NodeGraph* graph,
                   QWidget *parent,
                   const QString& isfName)
      : NodeBase(type, graph, parent, isfName)
{

}

QSize CropNode::getTargetSize() const
{
    QSize size(0, 0);
    size = getInputSize();

    auto vals = getAllPropertyValues().split(",");

    size.setWidth(std::max(1, size.width() - vals[0].toInt() - vals[2].toInt()));
    size.setHeight(std::max(1, size.height() - vals[1].toInt() - vals[3].toInt()));

    return size;
}

} // namespace Cascade
