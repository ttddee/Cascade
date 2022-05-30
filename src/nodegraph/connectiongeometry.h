#pragma once

#include "porttype.h"

#include <QtCore/QPointF>
#include <QtCore/QRectF>

#include <iostream>

namespace Cascade::NodeGraph
{

class ConnectionGeometry
{
public:
    ConnectionGeometry();

public:
    QPointF const& getEndPoint(PortType portType) const;

    void setEndPoint(PortType portType, QPointF const& point);

    void moveEndPoint(PortType portType, QPointF const &offset);

    QRectF boundingRect() const;

    std::pair<QPointF, QPointF> pointsC1C2() const;

    QPointF source() const { return mOutCoords; }
    QPointF sink() const { return mInCoords; }

    double lineWidth() const { return mLineWidth; }

    bool hovered() const { return mHovered; }
    void setHovered(bool hovered) { mHovered = hovered; }

private:
    // local object coordinates
    QPointF mInCoords;
    QPointF mOutCoords;

    double mLineWidth;

    bool mHovered;
};
}
