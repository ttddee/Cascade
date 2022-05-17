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
#include "../uientities/uientity.h"
#include "../log.h"
#include "../projectmanager.h"
#include "../global.h"

namespace Cascade {

NodeBase::NodeBase(
        const NodeType type,
        QWidget *parent,
        const QString& isfName)
    : QWidget(parent),
      ui(new Ui::NodeBase),
      mNodeType(type),
      mId(QUuid::createUuid().toString(QUuid::WithoutBraces))
{
    ui->setupUi(this);

    setUpNode(type);

    ProjectManager* pm = &ProjectManager::getInstance();
    connect(this, &NodeBase::nodeHasMoved,
            pm, &ProjectManager::handleProjectIsDirty);
    RenderManager* rm = &RenderManager::getInstance();
    connect(rm, &RenderManager::nodeHasBeenRendered,
            this, &NodeBase::onNodeHasBeenRendered);
}

void NodeBase::setUpNode(const NodeType nodeType)
{
    auto props = Cascade::getPropertiesForType(nodeType);

    QString label = props.title.toUpper();
    setLabeltext(label);

    createInputs(props);
    createOutputs(props);

    mNodeProperties = std::make_unique<NodeProperties>(nodeType, this, props);
}

void NodeBase::setLabeltext(const QString &text)
{
    ui->NodeTitleLabel->setText(text);
}

void NodeBase::createInputs(const NodeInitProperties &props)
{
    for (size_t i = 0; i < props.nodeInputs.size(); i++)
    {
        auto nodeIn = new NodeInput(props.nodeInputs[i], this);
        nodeIn->move(-2, 42 + 28 * i);
        mNodeInputs.push_back(nodeIn);

        QLabel *label;
        label = new QLabel(this);
        label->setObjectName("NodeIOLabel");
        label->move(nodeIn->pos() + QPoint(15, -7));
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (props.nodeInputs[i] == NodeInputType::eRgbBack)
        {
            mRgbaBackIn = nodeIn;
            label->setText("RGB Back");
        }
        else if (props.nodeInputs[i] == NodeInputType::eRgbFront)
        {
            mRgbaFrontIn = nodeIn;
            label->setText("RGB Front");
        }
        else if (props.nodeInputs[i] == NodeInputType::eRgbAlpha)
        {
            mRgbaFrontIn = nodeIn;
            label->setText("Alpha");
        }

        connect(nodeIn, &NodeInput::connectedNodeInputClicked,
                mNodeGraph, &NodeGraph::handleConnectedNodeInputClicked);
    }
}

void NodeBase::createOutputs(const NodeInitProperties &props)
{
    for (size_t i = 0; i < props.nodeOutputs.size(); i++)
    {
        auto nodeOut = new NodeOutput(this);
        nodeOut->move(112, 42);
        mNodeOutputs.push_back(nodeOut);

        QLabel *label;
        label = new QLabel(this);
        label->setObjectName("NodeIOLabel");
        label->move(nodeOut->pos() + QPoint(-25, -7));
        label->setText("Out");
        label->setAttribute(Qt::WA_TransparentForMouseEvents);

        if (props.nodeOutputs[i] == NodeOutputType::eRgb)
        {
            mRgbaOut = nodeOut;
        }
        connect(nodeOut, &NodeOutput::nodeOutputLeftMouseClicked,
                mNodeGraph, &NodeGraph::handleNodeOutputLeftClicked);
    }
}

bool NodeBase::operator==(const NodeBase* node) const
{
    if (node->getID() == mId)
        return true;
    return false;
}

bool NodeBase::getIsViewed() const
{
    return mIsViewed;
}

QString NodeBase::getID() const
{
    return mId;
}

void NodeBase::setID(const QString &s)
{
    mId = s;
}

void NodeBase::setInputIDs(const QMap<int, QString>& ids)
{
    for ( int i = 0; i < ids.size(); i++)
    {
        mNodeInputs[i]->setID(ids[i]);
    }
}

const std::vector<unsigned int>& NodeBase::getShaderCode()
{
    return mShaderCode;
}

void NodeBase::setShaderCode(const std::vector<unsigned int> code)
{
    mShaderCode = code;
}

void NodeBase::loadNodePropertyValues(const NodePersistentProperties& p)
{
    setID(p.uuid);
    setInputIDs(p.inputs);
    mNodeProperties->loadNodePropertyValues(p.properties);
}

NodeInput* NodeBase::findNodeInput(const QString& id)
{
    foreach(NodeInput* in, mNodeInputs)
    {
        if (in->getID() == id)
            return in;
    }
    return nullptr;
}

NodeOutput* NodeBase::getRgbaOut() const
{
    if (mRgbaOut)
    {
        return mRgbaOut;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeBack() const
{
    if(mRgbaBackIn && mRgbaBackIn->hasConnection())
    {
        return mRgbaBackIn->mInConnection->getSourceOutput()->mParentNode;
    }
    return nullptr;
}

NodeBase* NodeBase::getUpstreamNodeFront() const
{
    if(mRgbaFrontIn && mRgbaFrontIn->hasConnection())
    {
        return mRgbaFrontIn->mInConnection->getSourceOutput()->mParentNode;
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
    mNeedsUpdate = true;
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

NodeType NodeBase::getType() const
{
    return mNodeType;
}

bool NodeBase::getNeedsUpdate() const
{
    return mNeedsUpdate;
}

void NodeBase::setNeedsUpdate(const bool b)
{
    mNeedsUpdate = b;
}

QSize NodeBase::getTargetSize() const
{
    return getInputSize();;
}

void NodeBase::addNodeToJsonArray(QJsonArray& jsonNodesArray)
{
    QJsonObject jsonProps;

    auto widgets = getProperties()->mWidgets;

    for(size_t i = 0; i < widgets.size(); i++)
    {
        jsonProps.insert(QString::number(i), widgets[i]->getValuesAsString());
    }

    QJsonObject jsonInputs;
    for(size_t i = 0; i < mNodeInputs.size(); i++)
    {
        jsonInputs.insert(QString::number(i), mNodeInputs[i]->getID());
    }

    QJsonObject jsonNode {
        { "uuid", getID() },
        { "type", nodeStrings[mNodeType] },
        { "posx", pos().x() },
        { "posy", pos().y() },
        { "properties", jsonProps },
        { "inputs", jsonInputs },
        { "customname", mIsfName }
    };

    jsonNodesArray.push_back(jsonNode);
}

NodeInput* NodeBase::getOpenInput() const
{
    foreach(auto in, mNodeInputs)
    {
        if (!in->hasConnection())
            return in;
    }
    return nullptr;
}

QString NodeBase::getAllPropertyValues() const
{
    QString vals;
    auto widgets = getProperties()->mWidgets;

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
    if(mRgbaOut)
    {
        foreach(Connection* c, mRgbaOut->getConnections())
        {
            nodes.push_back(c->getTargetInput()->mParentNode);
            c->getTargetInput()->mParentNode->getAllDownstreamNodes(nodes);
        }
    }
}

std::set<Connection*> NodeBase::getAllConnections()
{
    std::set<Connection*> connections;

    if (mRgbaBackIn)
        if (auto c = mRgbaBackIn->getConnection())
            connections.insert(c);

    if (mRgbaFrontIn)
        if (auto c = mRgbaFrontIn->getConnection())
            connections.insert(c);

    if (mRgbaOut)
    {
        auto conns = mRgbaOut->getConnections();

        if (conns.size() > 0)
        {
            std::copy(conns.begin(), conns.end(), std::inserter(connections, connections.end()));
        }
    }
    return connections;
}

CsImage* NodeBase::getCachedImage() const
{
    if (mCachedImage)
        return mCachedImage.get();
    return nullptr;
}

void NodeBase::setCachedImage(std::unique_ptr<CsImage> image)
{
    mCachedImage = std::move(image);
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
    path.addRoundedRect(rect, Config::sNodeCornerRadius, Config::sNodeCornerRadius);
    painter.fillPath(path, mDefaultColorBrush);

    if (mIsSelected)
    {
        painter.setClipRect(0, 0, 120, 33, Qt::ReplaceClip);
        painter.fillPath(path, mSelectedColorBrush);
    }
    if (mIsViewed)
    {
        painter.setClipRect(0, 0, size().width(), size().height(), Qt::ReplaceClip);
        QPoint circlePos(size().width()- 18, size().height() - 18);
        painter.setBrush(mSelectedColorBrush);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(circlePos, 8, 8);
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
    for (NodeInput* in : mNodeInputs)
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
    if (mRgbaBackIn && !mRgbaBackIn->hasConnection())
    {
        return false;
    }
    return true;
}

NodeProperties* NodeBase::getProperties() const
{
    return mNodeProperties.get();
}

void NodeBase::updateConnectionPositions()
{
    foreach(NodeInput* nodeIn, mNodeInputs)
    {
        nodeIn->updateConnection();
    }
    foreach(NodeOutput* nodeOut, mNodeOutputs)
    {
        nodeOut->updateConnections();
    }
}

void NodeBase::flushCache()
{
    mCachedImage = nullptr;
}

void NodeBase::handleSetSelected(NodeBase* node, const bool b)
{
    if (this == node)
    {
        mIsSelected = b;
        update();
    }
}

void NodeBase::handleSetActive(NodeBase* node, const bool b)
{
    if (this == node)
    {
        mIsActive = b;
    }
}

void NodeBase::handleSetViewed(NodeBase* node, const bool b)
{
    if (this == node)
    {
        mIsViewed = b;
        update();
    }
}

void NodeBase::handleRequestNodeUpdate()
{
    requestUpdate();
}

void NodeBase::onNodeHasBeenRendered(Cascade::NodeBase* node)
{
    if (this == node)
    {
        setNeedsUpdate(false);
    }
}

void NodeBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!mIsDragging)
        {
            mIsDragging = true;
            mOldPos = event->globalPos();
        }
        emit nodeWasLeftClicked(this);
    }
    else if (event->button() == Qt::RightButton)
    {
        emit nodeWasRightClicked(this);
    }
}

void NodeBase::mouseMoveEvent(QMouseEvent *event)
{
    if (mIsDragging)
    {
        QPoint offset = event->globalPos() - mOldPos;
        QPoint newPos = pos() + (offset / mNodeGraph->getViewScale());
        move(newPos);
        mOldPos = event->globalPos();
        updateConnectionPositions();
    }
}

void NodeBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mIsDragging = false;
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
