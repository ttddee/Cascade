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

#include "nodegeometry.h"

#include <cmath>
#include <iostream>
#include <memory>

#include "node.h"
#include "nodedatamodel.h"
#include "nodegraphicsobject.h"
#include "nodestate.h"
#include "porttype.h"
#include "stylecollection.h"

using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeGeometry;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::PortType;

NodeGeometry::NodeGeometry(std::unique_ptr<NodeDataModel> const& dataModel)
    : mWidth(100)
    , mHeight(100)
    , mMinWidth(160)
    , mMinHeight(100)
    , mInputPortWidth(70)
    , mOutputPortWidth(70)
    , mEntryHeight(20)
    , mSpacing(12)
    , mHovered(false)
    , mNSources(dataModel->nPorts(PortType::Out))
    , mNSinks(dataModel->nPorts(PortType::In))
    , mDraggingPos(-1000, -1000)
    , mDataModel(dataModel)
    , mFontMetrics(QFont())
    , mBoldFontMetrics(QFont())
{
    QFont f;
    f.setBold(true);

    mBoldFontMetrics = QFontMetrics(f);
}

unsigned int NodeGeometry::nSources() const
{
    return mDataModel->nPorts(PortType::Out);
}

unsigned int NodeGeometry::nSinks() const
{
    return mDataModel->nPorts(PortType::In);
}

QRectF NodeGeometry::entryBoundingRect() const
{
    double const addon = 0.0;

    return QRectF(0 - addon, 0 - addon, mEntryWidth + 2 * addon, mEntryHeight + 2 * addon);
}

QRectF NodeGeometry::boundingRect() const
{
    auto const& nodeStyle = StyleCollection::nodeStyle();

    double addon = 4 * nodeStyle.ConnectionPointDiameter;

    return QRectF(0 - addon, 0 - addon, mWidth + 2 * addon, mHeight + 2 * addon);
}

void NodeGeometry::recalculateSize() const
{
    mEntryHeight = mFontMetrics.height();

    {
        unsigned int maxNumOfEntries = std::max(nSinks(), nSources());
        unsigned int step            = mEntryHeight + mSpacing;
        mHeight                      = step * maxNumOfEntries;
    }

    mHeight += captionHeight();

    mInputPortWidth  = portWidth(PortType::In);
    mOutputPortWidth = portWidth(PortType::Out);

    mWidth = mInputPortWidth + mOutputPortWidth + 2 * mSpacing;

    mWidth = std::max(mWidth, captionWidth());

    if (mDataModel->validationState() != NodeValidationState::Valid)
    {
        mWidth = std::max(mWidth, validationWidth());
        mHeight += validationHeight() + mSpacing;
    }

    mWidth  = std::max(mWidth, mMinWidth);
    mHeight = std::max(mHeight, mMinHeight);
}

void NodeGeometry::recalculateSize(QFont const& font) const
{
    QFontMetrics fontMetrics(font);
    QFont boldFont = font;

    boldFont.setBold(true);

    QFontMetrics boldFontMetrics(boldFont);

    if (mBoldFontMetrics != boldFontMetrics)
    {
        mFontMetrics     = fontMetrics;
        mBoldFontMetrics = boldFontMetrics;

        recalculateSize();
    }
}

QPointF NodeGeometry::portScenePosition(PortIndex index, PortType portType, QTransform const& t)
    const
{
    auto const& nodeStyle = StyleCollection::nodeStyle();

    unsigned int step = mEntryHeight + mSpacing;

    QPointF result;

    double totalHeight = 0.0;

    totalHeight += captionHeight();

    totalHeight += step * index;

    // TODO: why?
    totalHeight += step / 2.0;

    switch (portType)
    {
        case PortType::Out: {
            double x = mWidth + nodeStyle.ConnectionPointDiameter;

            result = QPointF(x, totalHeight);
            break;
        }

        case PortType::In: {
            double x = 0.0 - nodeStyle.ConnectionPointDiameter;

            result = QPointF(x, totalHeight);
            break;
        }

        default:
            break;
    }

    return t.map(result);
}

PortIndex NodeGeometry::checkHitScenePoint(
    PortType portType,
    QPointF const scenePoint,
    QTransform const& sceneTransform) const
{
    auto const& nodeStyle = StyleCollection::nodeStyle();

    PortIndex result = INVALID;

    if (portType == PortType::None)
        return result;

    double const tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;

    unsigned int const nItems = mDataModel->nPorts(portType);

    for (unsigned int i = 0; i < nItems; ++i)
    {
        auto pp = portScenePosition(i, portType, sceneTransform);

        QPointF p     = pp - scenePoint;
        auto distance = std::sqrt(QPointF::dotProduct(p, p));

        if (distance < tolerance)
        {
            result = PortIndex(i);
            break;
        }
    }

    return result;
}

QRect NodeGeometry::resizeRect() const
{
    unsigned int rectSize = 7;

    return QRect(mWidth - rectSize, mHeight - rectSize, rectSize, rectSize);
}

int NodeGeometry::equivalentWidgetHeight() const
{
    if (mDataModel->validationState() != NodeValidationState::Valid)
    {
        return height() - captionHeight() + validationHeight();
    }

    return height() - captionHeight();
}

unsigned int NodeGeometry::captionHeight() const
{
    QString name = mDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).height();
}

unsigned int NodeGeometry::captionWidth() const
{
    QString name = mDataModel->caption();

    return mBoldFontMetrics.boundingRect(name).width();
}

unsigned int NodeGeometry::validationHeight() const
{
    QString msg = mDataModel->validationMessage();

    return mBoldFontMetrics.boundingRect(msg).height();
}

unsigned int NodeGeometry::validationWidth() const
{
    QString msg = mDataModel->validationMessage();

    return mBoldFontMetrics.boundingRect(msg).width();
}

QPointF NodeGeometry::calculateNodePositionBetweenNodePorts(
    PortIndex targetPortIndex,
    PortType targetPort,
    Node* targetNode,
    PortIndex sourcePortIndex,
    PortType sourcePort,
    Node* sourceNode,
    Node& newNode)
{
    //Calculating the nodes position in the scene. It'll be positioned half way between the two ports that it "connects".
    //The first line calculates the halfway point between the ports (node position + port position on the node for both nodes averaged).
    //The second line offsets this coordinate with the size of the new node, so that the new nodes center falls on the originally
    //calculated coordinate, instead of it's upper left corner.
    auto converterNodePos =
        (sourceNode->nodeGraphicsObject().pos() +
         sourceNode->nodeGeometry().portScenePosition(sourcePortIndex, sourcePort) +
         targetNode->nodeGraphicsObject().pos() +
         targetNode->nodeGeometry().portScenePosition(targetPortIndex, targetPort)) /
        2.0f;
    converterNodePos.setX(converterNodePos.x() - newNode.nodeGeometry().width() / 2.0f);
    converterNodePos.setY(converterNodePos.y() - newNode.nodeGeometry().height() / 2.0f);
    return converterNodePos;
}

unsigned int NodeGeometry::portWidth(PortType portType) const
{
    unsigned width = 0;

    for (auto i = 0ul; i < mDataModel->nPorts(portType); ++i)
    {
        QString name;

        name = mDataModel->portCaption(portType, i);

        width = std::max(unsigned(mFontMetrics.horizontalAdvance(name)), width);
    }

    return width;
}
