/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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
*/

#include "nodebase.h"
#include "ui_nodebase.h"

#include <iostream>
#include <math.h>

#include <QPainter>
#include <QMouseEvent>
#include <QUuid>

#include "nodeinput.h"
#include "nodeoutput.h"
#include "nodegraph.h"
#include "connection.h"
#include "uientities/uientity.h"

NodeBase::NodeBase(
        const NodeType type,
        const NodeGraph* graph,
        const QString& gType,
        const QString& gHash,
        QWidget *parent)
    : QWidget(parent),
      nodeType(type),
      ui(new Ui::NodeBase),
      nodeGraph(graph),
      id(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    ui->setupUi(this);

    setGmicNodeType(gType);
    setGmicHash(gHash);

    this->setUpNode(type);

    wManager = &WindowManager::getInstance();
}

void NodeBase::setUpNode(const NodeType nodeType)
{
    NodeInitProperties props;

    props = Cascade::getPropertiesForType(nodeType);

    QString label = props.title;
    if (nodeType == NODE_TYPE_GMIC)
        label.append(" " + getGmicNodeType());
    ui->NodeTitleLabel->setText(label);

    this->createInputs(props);
    this->createOutputs(props);

    propertiesView = new NodeProperties(nodeType, this);
}

void NodeBase::createInputs(const NodeInitProperties &props)
{
    for (size_t i = 0; i < props.nodeInputs.size(); i++)
    {
        auto nodeIn = new NodeInput(props.nodeInputs[i], this);
        nodeIn->move(-2, 15 + 35 * i);
        nodeInputs.push_back(nodeIn);

        if (props.nodeInputs[i] == NODE_INPUT_TYPE_RGB_BACK)
        {
            this->rgbaBackIn = nodeIn;
        }
        else if (props.nodeInputs[i] == NODE_INPUT_TYPE_RGB_FRONT)
        {
            this->rgbaFrontIn = nodeIn;
        }
        else if (props.nodeInputs[i] == NODE_INPUT_TYPE_ALPHA)
        {
            this->rgbaFrontIn = nodeIn;
        }

        connect(nodeIn, &NodeInput::connectedNodeInputClicked,
                nodeGraph, &NodeGraph::handleConnectedNodeInputClicked);
    }
}

void NodeBase::createOutputs(const NodeInitProperties &props)
{
    for (size_t i = 0; i < props.nodeOutputs.size(); i++)
    {
        auto nodeOut = new NodeOutput(this);
        nodeOut->move(110, 15);
        nodeOutputs.push_back(nodeOut);

        if (props.nodeOutputs[i] == NODE_OUTPUT_TYPE_RGB)
        {
            //nodeOut->setObjectName("RGBOut");
            this->rgbaOut = nodeOut;
        }
        connect(nodeOut, &NodeOutput::nodeOutputLeftMouseClicked,
                nodeGraph, &NodeGraph::handleNodeOutputLeftClicked);
    }
}

bool NodeBase::getIsViewed() const
{
    return isViewed;
}

void NodeBase::setIsSelected(const bool b)
{
    isSelected = b;
    this->update();
}

void NodeBase::setIsActive(const bool b)
{
    isActive = b;
}

void NodeBase::setIsViewed(const bool b)
{
    isViewed = b;
}

QString NodeBase::getID() const
{
    return id;
}

NodeInput* NodeBase::getRgbaBackIn()
{
    if (rgbaBackIn)
    {
        return rgbaBackIn;
    }
    return nullptr;
}

NodeInput* NodeBase::getRgbaFrontIn()
{
    if (rgbaFrontIn)
    {
        return rgbaFrontIn;
    }
    return nullptr;
}

NodeOutput* NodeBase::getRgbaOut()
{
    if (rgbaOut)
    {
        return rgbaOut;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeBack()
{
    if(rgbaBackIn && rgbaBackIn->hasConnection())
    {
        return rgbaBackIn->inConnection->sourceOutput->parentNode;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeFront()
{
    if(rgbaFrontIn && rgbaFrontIn->hasConnection())
    {
        return rgbaFrontIn->inConnection->sourceOutput->parentNode;
    }
    return nullptr;
}

void NodeBase::getAllUpstreamNodes(std::vector<NodeBase*>& nodes)
{
    if(auto n = getUpstreamNodeBack())
    {
        n->getAllUpstreamNodes(nodes);
    }
    if(auto n = getUpstreamNodeFront())
    {
        n->getAllUpstreamNodes(nodes);
    }
    nodes.push_back(this);
}

void NodeBase::requestUpdate()
{
    if (nodeType == NODE_TYPE_CROP)
    {
        updateCropSizes();
    }
    else if (nodeType == NODE_TYPE_ROTATE)
    {
        updateRotation();
    }
    else if (nodeType == NODE_TYPE_RESIZE)
    {
        updateResizeFactor();
    }

    needsUpdate = true;
    invalidateAllDownstreamNodes();

    emit nodeRequestUpdate(this);
}

QString NodeBase::getCustomSize()
{
    return sizeSource->getValuesAsString();
}

bool NodeBase::getHasCustomSize()
{
    return hasCustomSize;
}

void NodeBase::setHasCustomSize(UiEntity* source)
{
    hasCustomSize = true;
    sizeSource = source;
}

QSize NodeBase::getTargetSize()
{
    if (hasCustomSize)
    {
        auto s = getCustomSize();

        if (s != "")
        {
            auto parts = s.split(",");
            QSize size(parts.at(0).toInt(), parts.at(1).toInt());

            return size;
        }
    }

    auto upstreamNode = getUpstreamNodeBack();

    if (upstreamNode)
    {
        auto upstreamImage = upstreamNode->cachedImage;

        if (upstreamImage)
        {
            int w = upstreamImage->getWidth();
            int h = upstreamImage->getHeight();

            if (nodeType == NODE_TYPE_CROP)
            {
                // Crop
                w = w - leftCrop - rightCrop;
                h = h - topCrop - bottomCrop;
                if(w < 0)
                {
                    w = 0;
                }
                if (h < 0)
                {
                    h = 0;
                }
            }

            // TODO: this
            // Rotation
//            if (rotation < 90 || (rotation > 180 && rotation < 270))
//            {
//                w = abs((w * cos(rotation) + (h * sin(rotation))));
//                h = abs((w * sin(rotation) + (h * cos(rotation))));
//            }
//            else
//            {
//                int temp = w;
//                w = h;
//                h = temp;

//                w = abs((w * cos(-rotation) + (h * sin(-rotation))));
//                h = abs((w * sin(-rotation) + (h * cos(-rotation))));
//            }

//            std::cout << "new size: " << w << " x " << h << std::endl;

            if (nodeType == NODE_TYPE_RESIZE)
            {
                // Resize
                w = static_cast<int>(w * resizeFactor);
                h = static_cast<int>(h * resizeFactor);
            }

            return QSize(w, h);
        }
    }
    return QSize(0, 0);
}

const QString& NodeBase::getGmicNodeType() const
{
    return gmicNodeType;
}

const QString& NodeBase::getGmicHash() const
{
    return gmicHash;
}

void NodeBase::setGmicNodeType(const QString &type)
{
    gmicNodeType = type;
}

void NodeBase::setGmicHash(const QString &hash)
{
    gmicHash = hash;
}

QString NodeBase::getAllPropertyValues()
{
    QString vals;
    auto widgets = getProperties()->widgets;

    for(size_t i = 0; i < widgets.size(); i++)
    {
        vals.append(widgets[i]->getValuesAsString());
        if(i != widgets.size() - 1)
        {
            vals.append(",");
        }
    }
    return vals;
}

void NodeBase::getAllDownstreamNodes(std::vector<NodeBase*>& nodes)
{    
    if(rgbaOut)
    {
        foreach(Connection* c, rgbaOut->getConnections())
        {
            nodes.push_back(c->targetInput->parentNode);
            c->targetInput->parentNode->getAllDownstreamNodes(nodes);
        }
    }
}

std::set<Connection*> NodeBase::getAllConnections()
{
    std::set<Connection*> connections;
    if (rgbaBackIn)
    {
        if (auto c = rgbaBackIn->getConnection())
        {
            connections.insert(c);
        }
    }
    if (rgbaFrontIn)
    {
        if (auto c = rgbaFrontIn->getConnection())
        {
            connections.insert(c);
        }
    }
    if (rgbaOut)
    {
        auto conns = rgbaOut->getConnections();

        if (conns.size() > 0)
        {
            foreach (auto& c, conns)
            {
                connections.insert(c);
            }
        }
    }
    return connections;
}

void NodeBase::invalidateAllDownstreamNodes()
{
    std::vector<NodeBase*> nodes;
    getAllDownstreamNodes(nodes);
    foreach(auto& n, nodes)
    {
        n->requestUpdate();
    }
}

void NodeBase::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    painter.setBrush(defaultColorBrush);
    painter.setPen(defaultColorPen);
    painter.drawRoundedRect(rect, cornerRadius, cornerRadius);

    auto mode = wManager->getViewerMode();

    if (isSelected)
    {
        painter.setBrush(selectedColorBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect, cornerRadius, cornerRadius);
    }
    if (isViewed)
    {
        rect.setTopLeft(rect.topLeft() + QPoint(1, 1));
        rect.setBottomRight(rect.bottomRight() + QPoint(-1, -1));

        if (mode == VIEWER_MODE_FRONT_RGB)
        {
            painter.setPen(frontViewedColorPen);
        }
        else if (mode == VIEWER_MODE_BACK_RGB)
        {
            painter.setPen(backViewedColorPen);
        }
        else if (mode == VIEWER_MODE_INPUT_ALPHA)
        {
            painter.setPen(alphaViewedColorPen);
        }
        else if (mode == VIEWER_MODE_OUTPUT_RGB)
        {
            painter.setPen(outputViewedColorPen);
        }
        else if (mode == VIEWER_MODE_OUTPUT_ALPHA)
        {
            painter.setPen(alphaViewedColorPen);
        }

        painter.drawRoundedRect(rect, cornerRadius - 2, cornerRadius - 2);
    }
    Q_UNUSED(event);
}

NodeInput* NodeBase::getNodeInputAtPosition(const QPoint position)
{
    for (NodeInput* in : nodeInputs)
    {
        auto bbox = QRect(mapToGlobal(in->geometry().topLeft()),
                          mapToGlobal(in->geometry().bottomRight()));
        if (bbox.contains(position))
        {
            return in;
        }
    }
    return nullptr;
}

bool NodeBase::canBeRendered()
{
    if (nodeType == NODE_TYPE_READ)
    {
        auto vals = getAllPropertyValues();
        if (vals.size() == 0)
        {
            return false;
        }
    }
    if (hasCustomSize && getCustomSize() != "")
    {
        return true;
    }
    if (rgbaBackIn && !rgbaBackIn->hasConnection())
    {
        return false;
    }
    return true;
}

NodeProperties* NodeBase::getProperties()
{
    return propertiesView;
}

void NodeBase::updateConnectionPositions()
{
    foreach(NodeInput* nodeIn, nodeInputs)
    {
        nodeIn->updateConnection();
    }
    foreach(NodeOutput* nodeOut, nodeOutputs)
    {
        nodeOut->updateConnections();
    }
}

void NodeBase::updateCropSizes()
{
    auto vals = getAllPropertyValues().split(",");
    leftCrop = vals[0].toInt();
    topCrop = vals[1].toInt();
    rightCrop = vals[2].toInt();
    bottomCrop = vals[3].toInt();
}

void NodeBase::updateRotation()
{
    auto vals = getAllPropertyValues().split(",");
    rotation = vals[0].toInt();
}

void NodeBase::updateResizeFactor()
{
    auto vals = getAllPropertyValues().split(",");
    resizeFactor = vals[0].toFloat();
}

void NodeBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!isDragging)
        {
            isDragging = true;
            oldPos = event->globalPos();
        }
        emit nodeWasLeftClicked(this);
    }
}

void NodeBase::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint offset = event->globalPos() - oldPos;
        QPoint newPos = this->pos() + (offset / nodeGraph->getViewScale());
        this->move(newPos);
        oldPos = event->globalPos();
        this->updateConnectionPositions();
    }
}

void NodeBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
}

void NodeBase::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit nodeWasDoubleClicked(this);

    Q_UNUSED(event);
}


NodeBase::~NodeBase()
{
    delete ui;
}
