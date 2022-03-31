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

#include "nodeinput.h"

#include <QMouseEvent>
#include <QUuid>

namespace Cascade {

NodeInput::NodeInput(NodeInputType t, QWidget *parent)
    : QPushButton (parent),
      inputType(t),
      id(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    parentNode = static_cast<NodeBase*>(parent);
    this->resize(10, 18);

    if (t == NODE_INPUT_TYPE_RGB_FRONT)
        setObjectName("Front");
    else if (t == NODE_INPUT_TYPE_RGB_BACK)
        setObjectName("Back");
    else if (t == NODE_INPUT_TYPE_ALPHA)
        setObjectName("Alpha");
}

NodeInputType NodeInput::getInputType()
{
    return inputType;
}

bool NodeInput::hasConnection()
{
    if(inConnection)
    {
        return true;
    }
    return false;
}

Connection* NodeInput::getConnection()
{
    if (hasConnection())
        return inConnection;

    return nullptr;
}

void NodeInput::updateConnection()
{
    if(inConnection)
    {
        inConnection->updatePosition();
    }
}

void NodeInput::addInConnection(Connection* c)
{
    inConnection = c;
    updateConnection();
    parentNode->requestUpdate();
}

void NodeInput::addInConnectionNoUpdate(Connection* c)
{
    inConnection = c;
    updateConnection();
}

void NodeInput::removeInConnection()
{
    inConnection = nullptr;
    parentNode->requestUpdate();
}

QString NodeInput::getID() const
{
    return id;
}

void NodeInput::setID(const QString &uuid)
{
    id = uuid;
}

void NodeInput::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (hasConnection())
        {
            emit connectedNodeInputClicked(inConnection);
        }
    }
}

} // namespace Cascade
