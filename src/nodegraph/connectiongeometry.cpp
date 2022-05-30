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

#include "connectiongeometry.h"

#include <cmath>

#include "stylecollection.h"

using Cascade::NodeGraph::ConnectionGeometry;
using Cascade::NodeGraph::PortType;

ConnectionGeometry::ConnectionGeometry() :
    mInCoords(0, 0),
    mOutCoords(0, 0),
    mLineWidth(3.0),
    mHovered(false)
{ }

QPointF const& ConnectionGeometry::getEndPoint(PortType portType) const
{
    Q_ASSERT(portType != PortType::None);

    return (portType == PortType::Out ?
                mOutCoords :
                mInCoords);
}


void ConnectionGeometry::setEndPoint(PortType portType, QPointF const& point)
{
    switch (portType)
    {
    case PortType::Out:
        mOutCoords = point;
        break;

    case PortType::In:
        mInCoords = point;
        break;

    default:
        break;
    }
}


void ConnectionGeometry::moveEndPoint(PortType portType, QPointF const &offset)
{
    switch (portType)
    {
    case PortType::Out:
        mOutCoords += offset;
        break;

    case PortType::In:
        mInCoords += offset;
        break;

    default:
        break;
    }
}


QRectF ConnectionGeometry::boundingRect() const
{
    auto points = pointsC1C2();

    QRectF basicRect = QRectF(mOutCoords, mInCoords).normalized();

    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    auto const &connectionStyle =
        StyleCollection::connectionStyle();

    float const diam = connectionStyle.pointDiameter();

    QRectF commonRect = basicRect.united(c1c2Rect);

    QPointF const cornerOffset(diam, diam);

    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}


std::pair<QPointF, QPointF> ConnectionGeometry::pointsC1C2() const
{
    const double defaultOffset = 200;

    double xDistance = mInCoords.x() - mOutCoords.x();

    double horizontalOffset = qMin(defaultOffset, std::abs(xDistance));

    double verticalOffset = 0;

    double ratioX = 0.5;

    if (xDistance <= 0)
    {
        double yDistance = mInCoords.y() - mOutCoords.y() + 20;

        double vector = yDistance < 0 ? -1.0 : 1.0;

        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

        ratioX = 1.0;
    }

    horizontalOffset *= ratioX;

    QPointF c1(mOutCoords.x() + horizontalOffset,
               mOutCoords.y() + verticalOffset);

    QPointF c2(mInCoords.x() - horizontalOffset,
               mInCoords.y() - verticalOffset);

    return std::make_pair(c1, c2);
}
