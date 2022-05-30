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
