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
      mInputType(t),
      mId(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    mParentNode = static_cast<NodeBase*>(parent);
    this->resize(10, 18);

    if (t == NodeInputType::eRgbFront)
        setObjectName("Front");
    else if (t == NodeInputType::eRgbBack)
        setObjectName("Back");
    else if (t == NodeInputType::eRgbAlpha)
        setObjectName("Alpha");
}

NodeInputType NodeInput::getInputType()
{
    return mInputType;
}

bool NodeInput::hasConnection()
{
    if(mInConnection)
    {
        return true;
    }
    return false;
}

Connection* NodeInput::getConnection()
{
    if (hasConnection())
        return mInConnection;

    return nullptr;
}

void NodeInput::updateConnection()
{
    if(mInConnection)
    {
        mInConnection->updatePosition();
    }
}

void NodeInput::addInConnection(Connection* c)
{
    mInConnection = c;
    updateConnection();
    mParentNode->requestUpdate();
}

void NodeInput::addInConnectionNoUpdate(Connection* c)
{
    mInConnection = c;
    updateConnection();
}

void NodeInput::removeInConnection()
{
    mInConnection = nullptr;
    mParentNode->requestUpdate();
}

QString NodeInput::getID() const
{
    return mId;
}

void NodeInput::setID(const QString &uuid)
{
    mId = uuid;
}

void NodeInput::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (hasConnection())
        {
            emit connectedNodeInputClicked(mInConnection);
        }
    }
}

} // namespace Cascade
