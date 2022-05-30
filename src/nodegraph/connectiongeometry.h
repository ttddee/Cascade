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
