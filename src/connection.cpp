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

#include "connection.h"

#include <math.h>

#include <QPen>
#include <QColor>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>

#include "nodeinput.h"
#include "nodeoutput.h"

// No M_PI on Windows
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

namespace Cascade {

Connection::Connection(NodeOutput* source)
    : QGraphicsLineItem(),
      mSourceOutput(source)
{
    this->setPen(mNormalPen);
    this->setLine(source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y(),
                  source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y());
    this->setZValue(-1);
    this->hide();
}

void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem*  opt, QWidget* wdgt)
{
    if(mTargetInput)
    {
        auto t = mTargetInput->getInputType();
        if (t == NodeInputType::eRgbFront)
            this->setPen(mFrontConnectedPen);
        else if (t == NodeInputType::eRgbBack)
            this->setPen(mBackConnectedPen);
        else if (t == NodeInputType::eRgbAlpha)
            this->setPen(mAlphaConnectedPen);
    }
    else
    {
        this->setPen(mNormalPen);
    }
    QGraphicsLineItem::paint(painter, opt, wdgt);
}

QPainterPath Connection::shape() const
{
    // Expand clickable region beyond line
    int selectionOffset = 10;
    QPainterPath ret;
    QPolygonF nPolygon;
    qreal radAngle = line().angle()* M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);
    nPolygon << line().p1() + offset1
             << line().p1() + offset2
             << line().p2() + offset2
             << line().p2() + offset1;
    ret.addPolygon(nPolygon);
    return ret;
}

void Connection::updatePosition()
{
    this->show();
    auto start = mSourceOutput->mParentNode->mapToParent(mSourceOutput->pos());

    this->setLine(start.x() + mSourceOutput->mVisualWidth / 2,
                  start.y() + mSourceOutput->mVisualHeight / 2,
                  mTargetInput->mParentNode->mapToParent(mTargetInput->pos()).x()
                  + mSourceOutput->mVisualWidth / 2,
                  mTargetInput->mParentNode->mapToParent(mTargetInput->pos()).y()
                  + mSourceOutput->mVisualHeight / 2);
}

void Connection::addConnectionToJsonObject(QJsonArray &jsonConnectionsArray)
{
    QJsonObject jsonConnection {
        { "src", mSourceOutput->mParentNode->getID() },
        { "dst-node", mTargetInput->mParentNode->getID() },
        { "dst", mTargetInput->getID()}
    };
    jsonConnectionsArray.push_back(jsonConnection);
}

void Connection::updatePosition(const QPoint end)
{
    this->show();
    auto start = mSourceOutput->mParentNode->mapToParent(mSourceOutput->pos());

    if (!mTargetInput)
    // Connection is open
    {
        this->setLine(start.x() + mSourceOutput->mVisualWidth / 2,
                      start.y() + mSourceOutput->mVisualHeight / 2,
                      end.x() - 5,
                      end.y() - 3);
    }
}

} //namespace Cascade

