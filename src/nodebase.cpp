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
*/

#include "nodebase.h"
#include "ui_nodebase.h"

#include <math.h>

#include <QPainter>
#include <QMouseEvent>
#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>

#include "nodeinput.h"
#include "nodeoutput.h"
#include "nodegraph.h"
#include "connection.h"
#include "uientities/uientity.h"
#include "log.h"
#include "projectmanager.h"
#include "isfmanager.h"

namespace Cascade {

NodeContextMenu* Cascade::NodeBase::contextMenu = nullptr;

NodeBase::NodeBase(
        const NodeType type,
        NodeGraph* graph,
        QWidget *parent,
        const QString& customName)
    : QWidget(parent),
      nodeType(type),
      ui(new Ui::NodeBase),
      nodeGraph(graph),
      id(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    ui->setupUi(this);

    this->setUpNode(type, customName);

    ProjectManager* pm = &ProjectManager::getInstance();
    connect(this, &NodeBase::nodeHasMoved,
            pm, &ProjectManager::handleProjectIsDirty);

    wManager = &WindowManager::getInstance();
}

void NodeBase::setUpNode(
        const NodeType nodeType,
        const QString& cName)
{
    NodeInitProperties props;

    if (nodeType != NODE_TYPE_ISF)
    {
        props = Cascade::getPropertiesForType(nodeType);
    }
    else
    {
        auto isfManager = &ISFManager::getInstance();
        props = isfManager->getNodeProperties().at(cName);
        this->setShaderCode(isfManager->getShaderCode(cName));
        customName = cName;
    }

    QString label = props.title.toUpper();
    ui->NodeTitleLabel->setText(label);

    this->createInputs(props);
    this->createOutputs(props);

    nodeProperties = std::make_unique<NodeProperties>(nodeType, this, props);
}

void NodeBase::createInputs(const NodeInitProperties &props)
{
    for (size_t i = 0; i < props.nodeInputs.size(); i++)
    {
        auto nodeIn = new NodeInput(props.nodeInputs[i], this);
        nodeIn->move(-2, 42 + 28 * i);
        nodeInputs.push_back(nodeIn);

        QLabel *label;
        label = new QLabel(this);
        label->setObjectName("NodeIOLabel");
        label->move(nodeIn->pos() + QPoint(15, -7));
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (props.nodeInputs[i] == NODE_INPUT_TYPE_RGB_BACK)
        {
            this->rgbaBackIn = nodeIn;
            label->setText("RGB Back");
        }
        else if (props.nodeInputs[i] == NODE_INPUT_TYPE_RGB_FRONT)
        {
            this->rgbaFrontIn = nodeIn;
            label->setText("RGB Front");
        }
        else if (props.nodeInputs[i] == NODE_INPUT_TYPE_ALPHA)
        {
            this->rgbaFrontIn = nodeIn;
            label->setText("Alpha");
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
        nodeOut->move(112, 42);
        nodeOutputs.push_back(nodeOut);

        QLabel *label;
        label = new QLabel(this);
        label->setObjectName("NodeIOLabel");
        label->move(nodeOut->pos() + QPoint(-25, -7));
        label->setText("Out");
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (props.nodeOutputs[i] == NODE_OUTPUT_TYPE_RGB)
        {
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

void NodeBase::setID(const QString &s)
{
    id = s;
}

void NodeBase::setInputIDs(const QMap<int, QString>& ids)
{
    for ( int i = 0; i < ids.size(); i++)
    {
        nodeInputs[i]->setID(ids[i]);
    }
}

const std::vector<unsigned int>& NodeBase::getShaderCode()
{
    return shaderCode;
}

void NodeBase::setShaderCode(const std::vector<unsigned int> code)
{
    shaderCode = code;
}

void NodeBase::loadNodePropertyValues(const QMap<int, QString> &values)
{
    nodeProperties->loadNodePropertyValues(values);
}

NodeInput* NodeBase::findNodeInput(const QString& id)
{
    foreach(NodeInput* in, nodeInputs)
    {
        if (in->getID() == id)
            return in;
    }
    return nullptr;
}

NodeInput* NodeBase::getRgbaBackIn() const
{
    if (rgbaBackIn)
    {
        return rgbaBackIn;
    }
    return nullptr;
}

NodeInput* NodeBase::getRgbaFrontIn() const
{
    if (rgbaFrontIn)
    {
        return rgbaFrontIn;
    }
    return nullptr;
}

NodeOutput* NodeBase::getRgbaOut() const
{
    if (rgbaOut)
    {
        return rgbaOut;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeBack() const
{
    if(rgbaBackIn && rgbaBackIn->hasConnection())
    {
        return rgbaBackIn->inConnection->sourceOutput->parentNode;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeFront() const
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

    needsUpdate = true;
    invalidateAllDownstreamNodes();

    emit nodeRequestUpdate(this);
}

QSize NodeBase::getInputSize() const
{
    auto upstreamNode = getUpstreamNodeBack();

    QSize size(0, 0);
    if (upstreamNode)
    {
        auto upstreamImage = upstreamNode->getCachedImage();

        if (upstreamImage)
        {
            size.setWidth(upstreamImage->getWidth());
            size.setHeight(upstreamImage->getHeight());
        }
    }
    return size;
}

QSize NodeBase::getTargetSize() const
{
    QSize size(0, 0);
    size = getInputSize();

    if (nodeType == NODE_TYPE_CROP)
    {
        // Crop
        size.setWidth(size.width() - leftCrop - rightCrop);
        size.setHeight(size.height() - topCrop - bottomCrop);
        if(size.width() < 0)
        {
            size.setWidth(0);
        }
        if (size.height() < 0)
        {
            size.setHeight(0);
        }
    }
    if (nodeType == NODE_TYPE_RESIZE)
    {
        // Resize
        auto vals = getAllPropertyValues().split(",");
        size.setWidth(vals[0].toInt());
        size.setHeight(vals[1].toInt());
    }
    return size;
}

void NodeBase::addNodeToJsonArray(QJsonArray& jsonNodesArray)
{
    QJsonObject jsonProps;

    auto widgets = getProperties()->widgets;

    for(size_t i = 0; i < widgets.size(); i++)
    {
        jsonProps.insert(QString::number(i), widgets[i]->getValuesAsString());
    }

    QJsonObject jsonInputs;
    for(size_t i = 0; i < nodeInputs.size(); i++)
    {
        jsonInputs.insert(QString::number(i), nodeInputs[i]->getID());
    }

    QJsonObject jsonNode {
        { "uuid", getID() },
        { "type", nodeStrings[nodeType] },
        { "posx", this->pos().x() },
        { "posy", this->pos().y() },
        { "properties", jsonProps },
        { "inputs", jsonInputs },
        { "customname", customName }
    };

    jsonNodesArray.push_back(jsonNode);
}

NodeInput* NodeBase::getOpenInput() const
{
    foreach(auto in, nodeInputs)
    {
        if (!in->hasConnection())
            return in;
    }
    return nullptr;
}

QString NodeBase::getAllPropertyValues() const
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
        if (auto c = rgbaBackIn->getConnection())
            connections.insert(c);

    if (rgbaFrontIn)
        if (auto c = rgbaFrontIn->getConnection())
            connections.insert(c);

    if (rgbaOut)
    {
        auto conns = rgbaOut->getConnections();

        if (conns.size() > 0)
        {
            std::copy(conns.begin(), conns.end(), std::inserter(connections, connections.end()));
        }
    }
    return connections;
}

CsImage* NodeBase::getCachedImage() const
{
    if (cachedImage)
        return cachedImage.get();
    return nullptr;
}

void NodeBase::setCachedImage(std::unique_ptr<CsImage> image)
{
    cachedImage = std::move(image);
}

void NodeBase::invalidateAllDownstreamNodes()
{
    std::vector<NodeBase*> nodes;
    getAllDownstreamNodes(nodes);
    foreach(auto& n, nodes)
    {
        n->flushCache();
        n->requestUpdate();
    }
}

void NodeBase::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, cornerRadius, cornerRadius);
    painter.fillPath(path, defaultColorBrush);

    if (isSelected)
    {
        painter.setClipRect(0, 0, 120, 33, Qt::ReplaceClip);
        painter.fillPath(path, selectedColorBrush);
    }

    Q_UNUSED(event);
}

void NodeBase::moveEvent(QMoveEvent *event)
{
    emit nodeHasMoved();

    QWidget::moveEvent(event);
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

bool NodeBase::canBeRendered() const
{
    if (nodeType == NODE_TYPE_READ)
    {
        auto vals = getAllPropertyValues();
        if (vals.size() == 0)
        {
            return false;
        }
    }
    else if (rgbaBackIn && !rgbaBackIn->hasConnection())
    {
        return false;
    }
    return true;
}

NodeProperties* NodeBase::getProperties() const
{
    return nodeProperties.get();
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

void NodeBase::showContextMenu()
{
    contextMenu = new NodeContextMenu(nodeGraph, this);
    contextMenu->exec(QCursor::pos());
}

void NodeBase::flushCache()
{
    cachedImage = nullptr;
}

const int NodeBase::getNumImages()
{
    return nodeProperties->getNumImages();
}

void NodeBase::switchToFirstImage()
{
    nodeProperties->switchToFirstImage();
}

void NodeBase::switchToNextImage()
{
    nodeProperties->switchToNextImage();
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
    else if (event->button() == Qt::RightButton)
    {
        this->showContextMenu();
        event->accept();
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

} // namespace Cascade
