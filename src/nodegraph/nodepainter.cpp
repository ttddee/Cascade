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

#include "nodepainter.h"

#include <cmath>

#include <QtCore/QMargins>

#include "node.h"
#include "nodedatamodel.h"
#include "nodegeometry.h"
#include "nodegraphicsobject.h"
#include "nodegraphscene.h"
#include "nodestate.h"
#include "porttype.h"
#include "stylecollection.h"

using Cascade::NodeGraph::Node;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeGeometry;
using Cascade::NodeGraph::NodeGraphicsObject;
using Cascade::NodeGraph::NodeGraphScene;
using Cascade::NodeGraph::NodePainter;
using Cascade::NodeGraph::NodeState;

void NodePainter::paint(QPainter* painter, Node& node, NodeGraphScene const& scene)
{
    NodeGeometry const& geom = node.nodeGeometry();

    NodeState const& state = node.nodeState();

    NodeGraphicsObject const& graphicsObject = node.nodeGraphicsObject();

    geom.recalculateSize(painter->font());

    //--------------------------------------------
    NodeDataModel const* model = node.nodeDataModel();

    drawNodeRect(painter, node, geom, model, graphicsObject);

    drawConnectionPoints(painter, geom, state, model, scene);

    drawFilledConnectionPoints(painter, geom, state, model);

    drawModelName(painter, geom, state, model);

    drawEntryLabels(painter, geom, state, model);

    drawResizeRect(painter, geom, model);

    drawValidationRect(painter, geom, model, graphicsObject);

    /// call custom painter
    if (auto painterDelegate = model->painterDelegate())
    {
        painterDelegate->paint(painter, geom, model);
    }
}

void NodePainter::drawNodeRect(
    QPainter* painter,
    Node& node,
    NodeGeometry const& geom,
    NodeDataModel const* model,
    NodeGraphicsObject const& graphicsObject)
{
    NodeStyle const& nodeStyle = model->nodeStyle();

    auto color = graphicsObject.isSelected() ? nodeStyle.SelectedBoundaryColor
                                             : nodeStyle.NormalBoundaryColor;

    color = node.getIsViewed() ? nodeStyle.ViewedBoundaryColor : color;

    if (geom.hovered())
    {
        QPen p(color, nodeStyle.HoveredPenWidth);
        painter->setPen(p);
    }
    else
    {
        QPen p(color, nodeStyle.PenWidth);
        painter->setPen(p);
    }

    QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(2.0, geom.height()));

    gradient.setColorAt(0.0, nodeStyle.GradientColor0);
    gradient.setColorAt(0.03, nodeStyle.GradientColor1);
    gradient.setColorAt(0.97, nodeStyle.GradientColor2);
    gradient.setColorAt(1.0, nodeStyle.GradientColor3);

    painter->setBrush(gradient);

    float diam = nodeStyle.ConnectionPointDiameter;

    QRectF boundary(-diam, -diam, 2.0 * diam + geom.width(), 2.0 * diam + geom.height());

    double const radius = 3.0;

    painter->drawRoundedRect(boundary, radius, radius);
}

void NodePainter::drawConnectionPoints(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeState const& state,
    NodeDataModel const* model,
     [[maybe_unused]] NodeGraphScene const& scene)
{
    NodeStyle const& nodeStyle  = model->nodeStyle();
    auto const& connectionStyle = StyleCollection::connectionStyle();

    float diameter       = nodeStyle.ConnectionPointDiameter;
    auto reducedDiameter = diameter * 0.6;

    for (PortType portType : {PortType::Out, PortType::In})
    {
        size_t n = state.getEntries(portType).size();

        for (unsigned int i = 0; i < n; ++i)
        {
            QPointF p = geom.portScenePosition(i, portType);

            bool canConnect =
                (state.getEntries(portType)[i].empty() ||
                 (portType == PortType::Out &&
                  model->portOutConnectionPolicy(i) == NodeDataModel::ConnectionPolicy::Many));

            double r = 1.0;
            if (state.isReacting() && canConnect && portType == state.reactingPortType())
            {

                auto diff   = geom.draggingPos() - p;
                double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                double const thres = 40.0;
                r                  = (dist < thres) ? (2.0 - dist / thres) : 1.0;
            }

            painter->setBrush(connectionStyle.normalColor(model->portCaption(portType, i)));

            painter->drawEllipse(p, reducedDiameter * r, reducedDiameter * r);
        }
    };
}

void NodePainter::drawFilledConnectionPoints(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeState const& state,
    NodeDataModel const* model)
{
    NodeStyle const& nodeStyle  = model->nodeStyle();
    auto const& connectionStyle = StyleCollection::connectionStyle();

    auto diameter = nodeStyle.ConnectionPointDiameter;

    for (PortType portType : {PortType::Out, PortType::In})
    {
        size_t n = state.getEntries(portType).size();

        for (size_t i = 0; i < n; ++i)
        {
            QPointF p =
                geom.portScenePosition(static_cast<PortIndex>(i), static_cast<PortType>(portType));

            if (!state.getEntries(portType)[i].empty())
            {
                painter->setBrush(connectionStyle.normalColor(model->portCaption(portType, i)));

                painter->drawEllipse(p, diameter * 0.4, diameter * 0.4);
            }
        }
    }
}

void NodePainter::drawModelName(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeState const& state,
    NodeDataModel const* model)
{
    NodeStyle const& nodeStyle = model->nodeStyle();

    Q_UNUSED(state);

    QString const& name = model->caption();

    QFont f = painter->font();

    f.setBold(true);

    QFontMetrics metrics(f);

    auto rect = metrics.boundingRect(name);

    QPointF position(
        (geom.width() - rect.width()) / 2.0, (geom.spacing() + geom.entryHeight()) / 3.0);

    painter->setFont(f);
    painter->setPen(nodeStyle.FontColor);
    painter->drawText(position, name);

    f.setBold(false);
    painter->setFont(f);
}

void NodePainter::drawEntryLabels(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeState const& state,
    NodeDataModel const* model)
{
    QFontMetrics const& metrics = painter->fontMetrics();

    for (PortType portType : {PortType::Out, PortType::In})
    {
        auto const& nodeStyle = model->nodeStyle();

        auto& entries = state.getEntries(portType);

        size_t n = entries.size();

        for (size_t i = 0; i < n; ++i)
        {
            QPointF p = geom.portScenePosition(static_cast<PortIndex>(i), portType);

            if (entries[i].empty())
                painter->setPen(nodeStyle.FontColorFaded);
            else
                painter->setPen(nodeStyle.FontColor);

            QString s = model->portCaption(portType, static_cast<PortIndex>(i));
            ;

            auto rect = metrics.boundingRect(s);

            p.setY(p.y() + rect.height() / 4.0);

            switch (portType)
            {
                case PortType::In:
                    p.setX(5.0);
                    break;

                case PortType::Out:
                    p.setX(geom.width() - 5.0 - rect.width());
                    break;

                default:
                    break;
            }

            painter->drawText(p, s);
        }
    }
}

void NodePainter::drawResizeRect(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeDataModel const* model)
{
    if (model->resizable())
    {
        painter->setBrush(Qt::gray);

        painter->drawEllipse(geom.resizeRect());
    }
}

void NodePainter::drawValidationRect(
    QPainter* painter,
    NodeGeometry const& geom,
    NodeDataModel const* model,
    NodeGraphicsObject const& graphicsObject)
{
    auto modelValidationState = model->validationState();

    if (modelValidationState != NodeValidationState::Valid)
    {
        NodeStyle const& nodeStyle = model->nodeStyle();

        auto color = graphicsObject.isSelected() ? nodeStyle.SelectedBoundaryColor
                                                 : nodeStyle.NormalBoundaryColor;

        if (geom.hovered())
        {
            QPen p(color, nodeStyle.HoveredPenWidth);
            painter->setPen(p);
        }
        else
        {
            QPen p(color, nodeStyle.PenWidth);
            painter->setPen(p);
        }

        //Drawing the validation message background
        if (modelValidationState == NodeValidationState::Error)
        {
            painter->setBrush(nodeStyle.ErrorColor);
        }
        else
        {
            painter->setBrush(nodeStyle.WarningColor);
        }

        double const radius = 3.0;

        float diam = nodeStyle.ConnectionPointDiameter;

        QRectF boundary(
            -diam,
            -diam + geom.height() - geom.validationHeight(),
            2.0 * diam + geom.width(),
            2.0 * diam + geom.validationHeight());

        painter->drawRoundedRect(boundary, radius, radius);

        painter->setBrush(Qt::gray);

        //Drawing the validation message itself
        QString const& errorMsg = model->validationMessage();

        QFont f = painter->font();

        QFontMetrics metrics(f);

        auto rect = metrics.boundingRect(errorMsg);

        QPointF position(
            (geom.width() - rect.width()) / 2.0,
            geom.height() - (geom.validationHeight() - diam) / 2.0);

        painter->setFont(f);
        painter->setPen(nodeStyle.FontColor);
        painter->drawText(position, errorMsg);
    }
}
