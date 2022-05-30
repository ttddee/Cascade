#include "nodegeometry.h"

#include "porttype.h"
#include "nodestate.h"
#include "nodedatamodel.h"
#include "node.h"
#include "nodegraphicsobject.h"
#include "stylecollection.h"

#include <QtGlobal>

#include <iostream>
#include <cmath>

using Cascade::NodeGraph::NodeGeometry;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::PortIndex;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::Node;

NodeGeometry::NodeGeometry(std::unique_ptr<NodeDataModel> const &dataModel) :
    mWidth(100),
    mHeight(150),
    mInputPortWidth(70),
    mOutputPortWidth(70),
    mEntryHeight(20),
    mSpacing(12),
    mHovered(false),
    mNSources(dataModel->nPorts(PortType::Out)),
    mNSinks(dataModel->nPorts(PortType::In)),
    mDraggingPos(-1000, -1000),
    mDataModel(dataModel),
    mFontMetrics(QFont()),
    mBoldFontMetrics(QFont())
{
    QFont f; f.setBold(true);

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

    return QRectF(0 - addon,
                  0 - addon,
                  mEntryWidth + 2 * addon,
                  mEntryHeight + 2 * addon);
}

QRectF NodeGeometry::boundingRect() const
{
    auto const &nodeStyle = StyleCollection::nodeStyle();

    double addon = 4 * nodeStyle.ConnectionPointDiameter;

    return QRectF(0 - addon,
                  0 - addon,
                  mWidth + 2 * addon,
                  mHeight + 2 * addon);
}


void NodeGeometry::recalculateSize() const
{
    mEntryHeight = mFontMetrics.height();

    {
        unsigned int maxNumOfEntries = std::max(mNSinks, mNSources);
        unsigned int step = mEntryHeight + mSpacing;
        mHeight = step * maxNumOfEntries;
    }

    if (auto w = mDataModel->embeddedWidget())
    {
        mHeight = std::max(mHeight, static_cast<unsigned>(w->height()));
    }

    mHeight += captionHeight();

    mInputPortWidth  = portWidth(PortType::In);
    mOutputPortWidth = portWidth(PortType::Out);

    mWidth = mInputPortWidth +
             mOutputPortWidth +
             2 * mSpacing;

    if (auto w = mDataModel->embeddedWidget())
    {
        mWidth += w->width();
    }

    mWidth = std::max(mWidth, captionWidth());

    if (mDataModel->validationState() != NodeValidationState::Valid)
    {
        mWidth   = std::max(mWidth, validationWidth());
        mHeight += validationHeight() + mSpacing;
    }
}


void NodeGeometry::recalculateSize(QFont const & font) const
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


QPointF NodeGeometry::portScenePosition(
    PortIndex index,
    PortType portType,
    QTransform const & t) const
{
    auto const &nodeStyle = StyleCollection::nodeStyle();

    unsigned int step = mEntryHeight + mSpacing;

    QPointF result;

    double totalHeight = 0.0;

    totalHeight += captionHeight();

    totalHeight += step * index;

    // TODO: why?
    totalHeight += step / 2.0;

    switch (portType)
    {
    case PortType::Out:
    {
        double x = mWidth + nodeStyle.ConnectionPointDiameter;

        result = QPointF(x, totalHeight);
        break;
    }

    case PortType::In:
    {
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
    QTransform const & sceneTransform) const
{
    auto const &nodeStyle = StyleCollection::nodeStyle();

    PortIndex result = INVALID;

    if (portType == PortType::None)
        return result;

    double const tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;

    unsigned int const nItems = mDataModel->nPorts(portType);

    for (unsigned int i = 0; i < nItems; ++i)
    {
        auto pp = portScenePosition(i, portType, sceneTransform);

        QPointF p = pp - scenePoint;
        auto    distance = std::sqrt(QPointF::dotProduct(p, p));

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

    return QRect(mWidth - rectSize,
                 mHeight - rectSize,
                 rectSize,
                 rectSize);
}


QPointF NodeGeometry::widgetPosition() const
{
    if (auto w = mDataModel->embeddedWidget())
    {
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            // If the widget wants to use as much vertical space as possible, place it immediately after the caption.
            return QPointF(mSpacing + portWidth(PortType::In), captionHeight());
        }
        else
        {
            if (mDataModel->validationState() != NodeValidationState::Valid)
            {
                return QPointF(mSpacing + portWidth(PortType::In),
                               (captionHeight() + mHeight - validationHeight() - mSpacing - w->height()) / 2.0);
            }

            return QPointF(mSpacing + portWidth(PortType::In),
                           (captionHeight() + mHeight - w->height()) / 2.0);
        }
    }
    return QPointF();
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
    auto converterNodePos = (sourceNode->nodeGraphicsObject().pos() + sourceNode->nodeGeometry().portScenePosition(sourcePortIndex, sourcePort) +
                             targetNode->nodeGraphicsObject().pos() + targetNode->nodeGeometry().portScenePosition(targetPortIndex, targetPort)) / 2.0f;
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        width = std::max(unsigned(mFontMetrics.horizontalAdvance(name)),
                         width);
#else
        width = std::max(unsigned(mFontMetrics.width(name)),
                         width);
#endif
    }

    return width;
}
