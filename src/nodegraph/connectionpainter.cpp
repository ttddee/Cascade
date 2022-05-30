#include "connectionpainter.h"

#include <QtGui/QIcon>

#include "connectiongeometry.h"
#include "connectionstate.h"
#include "connectiongraphicsobject.h"
#include "connection.h"
#include "porttype.h"
#include "nodedatamodel.h"
#include "node.h"

#include "nodedata.h"

#include "stylecollection.h"

using Cascade::NodeGraph::ConnectionPainter;
using Cascade::NodeGraph::ConnectionGeometry;
using Cascade::NodeGraph::Connection;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::Node;

static QPainterPath cubicPath(ConnectionGeometry const& geom)
{
    QPointF const& source = geom.source();
    QPointF const& sink   = geom.sink();

    auto c1c2 = geom.pointsC1C2();

    // cubic spline
    QPainterPath cubic(source);

    cubic.cubicTo(c1c2.first, c1c2.second, sink);

    return cubic;
}


QPainterPath ConnectionPainter:: getPainterStroke(ConnectionGeometry const& geom)
{
    auto cubic = cubicPath(geom);

    QPointF const& source = geom.source();
    QPainterPath result(source);

    unsigned segments = 20;

    for (auto i = 0ul; i < segments; ++i)
    {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker; stroker.setWidth(10.0);

    return stroker.createStroke(result);
}


#ifdef NODE_DEBUG_DRAWING
static void debugDrawing(
    QPainter * painter,
    Connection const & connection)
{
    Q_UNUSED(painter);
    Q_UNUSED(connection);
    ConnectionGeometry const& geom =
        connection.connectionGeometry();

    {
        QPointF const& source = geom.source();
        QPointF const& sink   = geom.sink();

        auto points = geom.pointsC1C2();

        painter->setPen(Qt::red);
        painter->setBrush(Qt::red);

        painter->drawLine(QLineF(source, points.first));
        painter->drawLine(QLineF(points.first, points.second));
        painter->drawLine(QLineF(points.second, sink));
        painter->drawEllipse(points.first, 3, 3);
        painter->drawEllipse(points.second, 3, 3);

        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubicPath(geom));
    }

    {
        painter->setPen(Qt::yellow);

        painter->drawRect(geom.boundingRect());
    }
}
#endif

static void drawSketchLine(
    QPainter * painter,
    Connection const & connection)
{
    using Cascade::NodeGraph::ConnectionState;

    ConnectionState const& state =
        connection.connectionState();

    if (state.requiresPort())
    {
        auto const & connectionStyle =
            Cascade::NodeGraph::StyleCollection::connectionStyle();

        QPen p;
        p.setWidth(connectionStyle.constructionLineWidth());
        p.setColor(connectionStyle.constructionColor());
        p.setStyle(Qt::DashLine);

        painter->setPen(p);
        painter->setBrush(Qt::NoBrush);

        using Cascade::NodeGraph::ConnectionGeometry;
        ConnectionGeometry const& geom = connection.connectionGeometry();

        auto cubic = cubicPath(geom);
        // cubic spline
        painter->drawPath(cubic);
    }
}


static void drawNormalLine(
    QPainter * painter,
    Connection const & connection)
{
    using Cascade::NodeGraph::ConnectionState;

    ConnectionState const& state =
        connection.connectionState();

    if (state.requiresPort())
        return;

    // colors

    auto const &connectionStyle = Cascade::NodeGraph::StyleCollection::connectionStyle();

    QColor normalColor  = connectionStyle.normalColor();
    QColor selectedColor = connectionStyle.selectedColor();

    if (connection.isComplete())
    {
        auto nodeModel = connection.getNode(PortType::In)->nodeDataModel();
        normalColor = connectionStyle.normalColor(
            nodeModel->portCaption(PortType::In, connection.getPortIndex(PortType::In)));
    }

    // geometry

    ConnectionGeometry const& geom = connection.connectionGeometry();

    double const lineWidth = connectionStyle.lineWidth();

    // draw normal line
    QPen p;

    p.setWidth(lineWidth);

    auto const& graphicsObject = connection.getConnectionGraphicsObject();
    bool const selected = graphicsObject.isSelected();

    auto cubic = cubicPath(geom);

    p.setColor(normalColor);

    if (selected)
    {
        p.setColor(selectedColor);
    }

    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);

    painter->drawPath(cubic);
}


void ConnectionPainter::paint(
    QPainter* painter,
    Connection const &connection)
{
    drawSketchLine(painter, connection);

    drawNormalLine(painter, connection);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, connection);
#endif

    // draw end points
    ConnectionGeometry const& geom = connection.connectionGeometry();

    QPointF const & source = geom.source();
    QPointF const & sink   = geom.sink();

    auto const & connectionStyle =
        Cascade::NodeGraph::StyleCollection::connectionStyle();

    double const pointDiameter = connectionStyle.pointDiameter();

    painter->setPen(connectionStyle.constructionColor());
    painter->setBrush(connectionStyle.constructionColor());
    double const pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(source, pointRadius, pointRadius);
    painter->drawEllipse(sink, pointRadius, pointRadius);
}
