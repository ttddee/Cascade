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

#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtGui/QFontMetrics>
#include <QtGui/QTransform>

#include "memory.h"
#include "porttype.h"

namespace Cascade::NodeGraph
{

class NodeState;
class NodeDataModel;
class Node;

class NodeGeometry
{
public:
    NodeGeometry(std::unique_ptr<NodeDataModel> const& dataModel);

public:
    unsigned int height() const
    {
        return mHeight;
    }

    void setHeight(unsigned int h)
    {
        mHeight = h;
    }

    unsigned int width() const
    {
        return mWidth;
    }

    void setWidth(unsigned int w)
    {
        mWidth = w;
    }

    unsigned int entryHeight() const
    {
        return mEntryHeight;
    }

    void setEntryHeight(unsigned int h)
    {
        mEntryHeight = h;
    }

    unsigned int entryWidth() const
    {
        return mEntryWidth;
    }

    void setEntryWidth(unsigned int w)
    {
        mEntryWidth = w;
    }

    unsigned int spacing() const
    {
        return mSpacing;
    }

    void setSpacing(unsigned int s)
    {
        mSpacing = s;
    }

    bool hovered() const
    {
        return mHovered;
    }

    void setHovered(unsigned int h)
    {
        mHovered = h;
    }

    unsigned int nSources() const;

    unsigned int nSinks() const;

    QPointF const& draggingPos() const
    {
        return mDraggingPos;
    }

    void setDraggingPosition(QPointF const& pos)
    {
        mDraggingPos = pos;
    }

public:
    QRectF entryBoundingRect() const;

    QRectF boundingRect() const;

    /// Updates size unconditionally
    void recalculateSize() const;

    /// Updates size if the QFontMetrics is changed
    void recalculateSize(QFont const& font) const;

    // TODO removed default QTransform()
    QPointF portScenePosition(
        PortIndex index,
        PortType portType,
        QTransform const& t = QTransform()) const;

    PortIndex checkHitScenePoint(
        PortType portType,
        QPointF point,
        QTransform const& t = QTransform()) const;

    QRect resizeRect() const;

    /// Returns the maximum height a widget can be without causing the node to grow.
    int equivalentWidgetHeight() const;

    unsigned int validationHeight() const;

    unsigned int validationWidth() const;

    static QPointF calculateNodePositionBetweenNodePorts(
        PortIndex targetPortIndex,
        PortType targetPort,
        Node* targetNode,
        PortIndex sourcePortIndex,
        PortType sourcePort,
        Node* sourceNode,
        Node& newNode);

private:
    unsigned int captionHeight() const;

    unsigned int captionWidth() const;

    unsigned int portWidth(PortType portType) const;

private:
    // some variables are mutable because
    // we need to change drawing metrics
    // corresponding to fontMetrics
    // but this doesn't change constness of Node

    mutable unsigned int mWidth;
    mutable unsigned int mHeight;
    unsigned int mMinWidth;
    unsigned int mMinHeight;
    unsigned int mEntryWidth;
    mutable unsigned int mInputPortWidth;
    mutable unsigned int mOutputPortWidth;
    mutable unsigned int mEntryHeight;
    unsigned int mSpacing;

    bool mHovered;

    unsigned int mNSources;
    unsigned int mNSinks;

    QPointF mDraggingPos;

    std::unique_ptr<NodeDataModel> const& mDataModel;

    mutable QFontMetrics mFontMetrics;
    mutable QFontMetrics mBoldFontMetrics;
};
} // namespace Cascade::NodeGraph
