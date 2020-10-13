/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QGraphicsItem>

#include "nodebase.h"
#include "uicolors.h"

class Connection : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    explicit Connection(NodeOutput* source);
    QPainterPath shape() const override;
    QPoint getStartPosition();

    void updatePosition();
    void updatePosition(const QPoint end);

    NodeInput* targetInput = nullptr;
    NodeOutput* sourceOutput = nullptr;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*  opt, QWidget* wdgt) override;

    const QPen normalPen = QPen(QColor("#9299a1"), 3);
    const QPen frontConnectedPen = QPen(frontColor, 3);
    const QPen backConnectedPen = QPen(backColor, 3);
    const QPen alphaConnectedPen = QPen(alphaColor, 3);

signals:
    void requestConnectionDeletion(Connection* c);

};

#endif // CONNECTION_H
