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

#include "nodeoutput.h"

#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QPen>
#include <QColor>

#include "../log.h"

namespace Cascade {

NodeOutput::NodeOutput(QWidget *parent)
    : QPushButton (parent)
{
    // Keep reference to parent node
    mParentNode = static_cast<NodeBase*>(parent);

    this->resize(mVisualWidth, mVisualHeight);

    setObjectName("Output");
}

void NodeOutput::addConnection(Connection *c)
{
    mOutConnections.push_back(c);
}

void NodeOutput::removeConnection(Connection *c)
{
    for(size_t i = 0; i < mOutConnections.size(); ++i)
    {
        if (c == mOutConnections[i])
        {
            mOutConnections.erase(mOutConnections.begin() + i);
        }
    }
}

void NodeOutput::updateConnections()
{
    foreach(Connection* c, mOutConnections)
    {
        c->updatePosition();
    }
}

void NodeOutput::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit nodeOutputLeftMouseClicked(this);
        //this->isDragging = true;
    }
}

std::vector<Connection*> NodeOutput::getConnections()
{
    return mOutConnections;
}

} // namespace Cascade

