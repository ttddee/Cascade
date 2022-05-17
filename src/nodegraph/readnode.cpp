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

#include "readnode.h"

#include "../log.h"

namespace Cascade {

ReadNode::ReadNode(const NodeType type,
                   QWidget *parent,
                   const QString& customName)
    : NodeBase(type, parent, customName)
{

}

bool ReadNode::canBeRendered() const
{
    auto vals = getAllPropertyValues();
    if (vals.size() == 0)
    {
        return false;
    }
    return true;
}

const int ReadNode::getNumImages()
{
    return mNodeProperties->getNumImages();
}

void ReadNode::switchToFirstImage()
{
    mNodeProperties->switchToFirstImage();
}

void ReadNode::switchToNextImage()
{
    mNodeProperties->switchToNextImage();
}

} // namespace Cascade
