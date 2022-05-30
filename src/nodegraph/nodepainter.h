#pragma once

#include <QtGui/QPainter>

namespace Cascade::NodeGraph
{

class Node;
class NodeState;
class NodeGeometry;
class NodeGraphicsObject;
class NodeDataModel;
class FlowItemEntry;
class NodeGraphScene;

class NodePainter
{
public:
    NodePainter();

public:
    static void paint(
        QPainter* painter,
        Node& node,
        NodeGraphScene const& scene);

    static void drawNodeRect(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeDataModel const* model,
        NodeGraphicsObject const & graphicsObject);

    static void drawModelName(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeState const& state,
        NodeDataModel const * model);

    static void drawEntryLabels(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeState const& state,
        NodeDataModel const * model);

    static void drawConnectionPoints(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeState const& state,
        NodeDataModel const * model,
        NodeGraphScene const & scene);

    static void drawFilledConnectionPoints(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeState const& state,
        NodeDataModel const * model);

    static void drawResizeRect(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeDataModel const * model);

    static void drawValidationRect(
        QPainter * painter,
        NodeGeometry const & geom,
        NodeDataModel const * model,
        NodeGraphicsObject const & graphicsObject);
};
}
