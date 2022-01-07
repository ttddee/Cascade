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
#include <iostream>

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

Connection::Connection(NodeOutput* source)
    : QGraphicsLineItem(),
      sourceOutput(source)
{
    // TODO find better way to get cursor position
    this->setPen(normalPen);
    this->setLine(source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y(),
                  source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y());
    this->setZValue(-1);
    this->hide();
}

void Connection::paint(QPainter* painter, const QStyleOptionGraphicsItem*  opt, QWidget* wdgt)
{
    if(targetInput)
    {
        auto t = targetInput->getInputType();
        if (t == NODE_INPUT_TYPE_RGB_FRONT)
            this->setPen(frontConnectedPen);
        else if (t == NODE_INPUT_TYPE_RGB_BACK)
            this->setPen(backConnectedPen);
        else if (t == NODE_INPUT_TYPE_ALPHA)
            this->setPen(alphaConnectedPen);
    }
    else
    {
        this->setPen(normalPen);
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
    auto start = sourceOutput->parentNode->mapToParent(sourceOutput->pos());

    this->setLine(start.x() + sourceOutput->visualWidth / 2,
                  start.y() + sourceOutput->visualHeight / 2,
                  targetInput->parentNode->mapToParent(targetInput->pos()).x()
                  + sourceOutput->visualWidth / 2,
                  targetInput->parentNode->mapToParent(targetInput->pos()).y()
                  + sourceOutput->visualHeight / 2);
}

void Connection::addConnectionToJsonObject(QJsonArray &connectionList)
{
    //connectionList.insert(sourceOutput->parentNode->getID(), targetInput->getID());
}

void Connection::updatePosition(const QPoint end)
{
    this->show();
    auto start = sourceOutput->parentNode->mapToParent(sourceOutput->pos());

    if (!targetInput)
    // Connection is open
    {
        this->setLine(start.x() + sourceOutput->visualWidth / 2,
                      start.y() + sourceOutput->visualHeight / 2,
                      end.x() - 5,
                      end.y() - 3);
    }

}

