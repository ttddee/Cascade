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

#include "connection.h"

#include "nodegeometry.h"
#include "nodestate.h"

class QGraphicsProxyWidget;

namespace Cascade::NodeGraph
{

class NodeGraphDataModel;
class NodeGraphScene;
class FlowItemEntry;

/// Class reacts on GUI events, mouse clicks and
/// forwards painting operation.
class NodeGraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    NodeGraphicsObject(
        NodeGraphDataModel &model,
        NodeGraphScene &scene,
        Node& node);

    virtual ~NodeGraphicsObject();

    Node& node();

    Node const& node() const;

    QRectF boundingRect() const override;

    void setGeometryChanged();

    /// Visits all attached connections and corrects
    /// their corresponding end points.
    void moveConnections() const;

    enum { Type = UserType + 1 };

    int type() const override { return Type; }

    void lock(bool locked);

protected:
    void paint(
        QPainter*                       painter,
        QStyleOptionGraphicsItem const* option,
        QWidget*                        widget = 0) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverMoveEvent(QGraphicsSceneHoverEvent *) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    void embedQWidget();

private:

    NodeGraphDataModel& mModel;
    NodeGraphScene& mScene;

    Node& mNode;

    bool mLocked;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget * mProxyWidget;
};
}
