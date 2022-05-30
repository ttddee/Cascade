#pragma once

#include <QPainter>

#include "nodegeometry.h"
#include "nodedatamodel.h"

namespace Cascade::NodeGraph {

/// Class to allow for custom painting
class NodePainterDelegate
{

public:
    virtual ~NodePainterDelegate() = default;

    virtual void paint(
        QPainter* painter,
        NodeGeometry const& geom,
        NodeDataModel const * model) = 0;
};
}
