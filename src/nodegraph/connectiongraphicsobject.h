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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#pragma once

#include <QtCore/QUuid>

#include <QtWidgets/QGraphicsObject>

class QGraphicsSceneMouseEvent;

namespace Cascade::NodeGraph
{

class NodeGraphDataModel;
class NodeGraphScene;
class Connection;
class ConnectionGeometry;
class Node;

/// Graphic Object for connection. Adds itself to scene
class ConnectionGraphicsObject
    : public QGraphicsObject
{
    Q_OBJECT

public:
    ConnectionGraphicsObject(
        NodeGraphDataModel& model,
        NodeGraphScene& scene,
        Connection &connection);

    virtual ~ConnectionGraphicsObject();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

public:
    Connection& connection();

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    void setGeometryChanged();

    /// Updates the position of both ends
    void move();

    void lock(bool locked);

protected:

    void paint(
        QPainter* painter,
        QStyleOptionGraphicsItem const* option,
        QWidget* widget = 0) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:

    void addGraphicsEffect();

private:

    NodeGraphDataModel& mModel;
    NodeGraphScene& mScene;

    Connection& mConnection;
};
}
