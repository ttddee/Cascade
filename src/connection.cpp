#include "connection.h"

#include <math.h>
#include <iostream>

#include <QPen>
#include <QColor>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>

#include "nodeinput.h"
#include "nodeoutput.h"

Connection::Connection(NodeOutput* source)
    : QGraphicsLineItem(),
      sourceOutput(source)
{
    // TODO find better way to get cursor position
    this->setPen(QPen(QColor("#9299a1"), 3));
    this->setLine(source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y(),
                  source->mapToParent(source->pos()).x(),
                  source->mapToParent(source->pos()).y());
    this->setZValue(-1);
}

//void Connection::attachToTarget(NodeIn *target)
//{
//    targetIn = target;
//    target->addInConnection(this);
//    this->setPen(QPen(QColor("#9bcf43"), 3));
//}

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

QPoint Connection::getStartPosition()
{
    return sourceOutput->mapToParent(line().p1().toPoint());
}

QPoint Connection::getEndPosition()
{

}

void Connection::updatePosition()
{
    auto start = sourceOutput->parentNode->mapToParent(sourceOutput->pos());

    this->setLine(start.x() + sourceOutput->visualWidth / 2,
                  start.y() + sourceOutput->visualHeight / 2,
                  targetInput->parentNode->mapToParent(targetInput->pos()).x()
                  + sourceOutput->visualWidth / 2,
                  targetInput->parentNode->mapToParent(targetInput->pos()).y()
                  + sourceOutput->visualHeight / 2);
}

void Connection::updatePosition(const QPoint end)
{
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
