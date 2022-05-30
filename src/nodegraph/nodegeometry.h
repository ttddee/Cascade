#pragma once

#include <QtCore/QRectF>
#include <QtCore/QPointF>
#include <QtGui/QTransform>
#include <QtGui/QFontMetrics>

#include "porttype.h"
#include "memory.h"

namespace Cascade::NodeGraph
{

class NodeState;
class NodeDataModel;
class Node;

class NodeGeometry
{
public:
    NodeGeometry(std::unique_ptr<NodeDataModel> const &dataModel);

public:
    unsigned int height() const { return mHeight; }

    void setHeight(unsigned int h) { mHeight = h; }

    unsigned int width() const { return mWidth; }

    void setWidth(unsigned int w) { mWidth = w; }

    unsigned int entryHeight() const { return mEntryHeight; }
    void setEntryHeight(unsigned int h) { mEntryHeight = h; }

    unsigned int entryWidth() const { return mEntryWidth; }

    void setEntryWidth(unsigned int w) { mEntryWidth = w; }

    unsigned int spacing() const { return mSpacing; }

    void setSpacing(unsigned int s) { mSpacing = s; }

    bool hovered() const { return mHovered; }

    void setHovered(unsigned int h) { mHovered = h; }

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
    void recalculateSize(QFont const &font) const;

    // TODO removed default QTransform()
    QPointF portScenePosition(
        PortIndex index,
        PortType portType,
        QTransform const & t = QTransform()) const;

    PortIndex checkHitScenePoint(
        PortType portType,
        QPointF point,
        QTransform const & t = QTransform()) const;

    QRect resizeRect() const;

    /// Returns the position of a widget on the Node surface
    QPointF widgetPosition() const;

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
}
