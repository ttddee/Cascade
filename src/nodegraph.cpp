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

#include "nodegraph.h"

#include <iostream>

#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsProxyWidget>

#include "nodeinput.h"
#include "nodeoutput.h"

NodeGraph::NodeGraph(QWidget* parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);

    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);
    this->setSceneRect(0, 0, viewWidth, viewHeight);
    this->centerOn(viewWidth / 2, viewHeight / 2);

    wManager = &WindowManager::getInstance();
    rManager = &RenderManager::getInstance();

    contextMenu = new NodeGraphContextMenu(this);

    connect(this, &NodeGraph::requestNodeDisplay,
            rManager, &RenderManager::handleNodeDisplayRequest);
    connect(this, &NodeGraph::requestNodeFileSave,
            rManager, &RenderManager::handleNodeFileSaveRequest);
    connect(this, &NodeGraph::requestClearScreen,
            rManager, &RenderManager::handleClearScreenrequest);
}

void NodeGraph::createNode(const NodeType type)
{
    NodeBase* n = new NodeBase(type, this);
    scene->addWidget(n);
    n->move(mapToScene(lastMousePos).x(),
            mapToScene(lastMousePos).y());
    nodes.push_back(n);

    connect(n, &NodeBase::nodeWasLeftClicked,
                this, &NodeGraph::handleNodeLeftClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            this, &NodeGraph::handleNodeDoubleClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            wManager, &WindowManager::handleNodeDoubleClicked);
    connect(n, &NodeBase::nodeRequestUpdate,
            this, &NodeGraph::handleNodeUpdateRequest);
    if (type == NODE_TYPE_WRITE)
    {
        connect(n, &NodeBase::nodeRequestFileSave,
                this, &NodeGraph::handleFileSaveRequest);
    }
}

void NodeGraph::deleteNode(NodeBase *node)
{
    node->invalidateAllDownstreamNodes();

    auto connections = node->getAllConnections();
    foreach (auto& c, connections)
    {
        deleteConnection(c);
    }

    nodes.erase(remove(nodes.begin(), nodes.end(), node), nodes.end());

    scene->removeItem(node->graphicsProxyWidget());

    if (node == viewedNode)
    {
        emit requestClearScreen();
    }

    delete node;

    selectedNode = nullptr;
}

float NodeGraph::getViewScale() const
{
    return viewScale;
}

void NodeGraph::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

QGraphicsItem* NodeGraph::getObjectUnderCursor()
{
    auto item = scene->itemAt(mapToScene(mapFromGlobal(QCursor::pos())), QTransform());

    return item;
}

QWidget* NodeGraph::getWidgetFromGraphicsitem(QGraphicsItem *item)
{
    QGraphicsProxyWidget* pWidget = qgraphicsitem_cast<QGraphicsProxyWidget*>(item);

    if(pWidget)
    {
        return pWidget->widget();
    }
    return nullptr;
}

void NodeGraph::selectNode(NodeBase *node)
{
    selectedNode = node;
    foreach(NodeBase* n, nodes)
    {
        n->setIsSelected(false);
    }
    node->setIsSelected(true);
}

void NodeGraph::activateNode(NodeBase *node)
{
    activeNode = node;
    node->setIsActive(true);
}

void NodeGraph::viewNode(NodeBase *node)
{
    if (node)
    {
        viewedNode = node;

        foreach(NodeBase* n, nodes)
        {
            n->setIsViewed(false);
            n->repaint();
        }
        node->setIsViewed(true);
        node->repaint();

        emit requestNodeDisplay(node);
    }
}

NodeBase* NodeGraph::getViewedNode()
{
    return viewedNode;
}

void NodeGraph::handleNodeLeftClicked(NodeBase* node)
{
    selectNode(node);
}

void NodeGraph::handleNodeDoubleClicked(NodeBase* node)
{
    if (node)
    {
        activateNode(node);
    }
    else
    {
        activeNode = nullptr;
    }
}

void NodeGraph::handleNodeOutputLeftClicked(NodeOutput* nodeOut)
{
    leftMouseIsDragging = true;

    createOpenConnection(nodeOut);
}

void NodeGraph::handleNodeUpdateRequest(NodeBase* node)
{
    if (node->getIsViewed())
    {
        std::cout << "requesting node display" << std::endl;

        emit requestNodeDisplay(node);
    }
}

void NodeGraph::handleFileSaveRequest(NodeBase* node, const QString& path)
{
    viewNode(node);
    emit requestNodeFileSave(node, path);
}

Connection* NodeGraph::createOpenConnection(NodeOutput* nodeOut)
{
    Connection* c = new Connection(nodeOut);
    scene->addItem(c);
    openConnection = c;

    connect(c, &Connection::requestConnectionDeletion,
            this, &NodeGraph::handleConnectionDeletionRequest);

    return c;
}

void NodeGraph::destroyOpenConnection()
{
    if(openConnection)
    {
        scene->removeItem(openConnection);
        delete openConnection;
        openConnection = nullptr;
    }
}

void NodeGraph::establishConnection(NodeInput *nodeIn)
{
    openConnection->targetInput = nodeIn;
    openConnection->sourceOutput->addConnection(openConnection);
    connections.push_back(openConnection);
    nodeIn->addInConnection(openConnection);
    openConnection = nullptr;
}

void NodeGraph::deleteConnection(Connection* c)
{
    for(size_t i = 0; i < connections.size(); ++i)
    {
        if (c == connections[i])
        {
            connections.erase(connections.begin() + i);
        }
    }

    c->sourceOutput->removeConnection(c);
    c->targetInput->removeInConnection();

    scene->removeItem(c);
    delete c;
    c = nullptr;
}

NodeBase* NodeGraph::getSelectedNode()
{
    return selectedNode;
}

void NodeGraph::handleConnectionDeletionRequest(Connection* c)
{
    deleteConnection(c);
}

void NodeGraph::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        foreach(NodeBase* n, nodes)
        {
            n->setIsSelected(false);
        }
    }
    if (event->button() == Qt::RightButton)
    {
        showContextMenu(event->pos());
    }
    else if (event->button() == Qt::MiddleButton)
    {
        middleMouseIsDragging = true;
    }
    lastMousePos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void NodeGraph::mouseMoveEvent(QMouseEvent* event)
{
    if (middleMouseIsDragging)
    // Scroll the whole scene
    {
        auto t = event->pos() - lastMousePos;
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - t.x());
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - t.y());
    }
    else if(leftMouseIsDragging)
    {
        if(openConnection)
        {
            openConnection->updatePosition(QPoint(
                                           mapToScene(mapFromGlobal(QCursor::pos())).x(),
                                           mapToScene(mapFromGlobal(QCursor::pos())).y()));
        }
    }
    lastMousePos = event->pos();

    QGraphicsView::mouseMoveEvent(event);
}

void NodeGraph::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftMouseIsDragging = false;

        auto item = getObjectUnderCursor();

        NodeBase* node = qobject_cast<NodeBase*>(getWidgetFromGraphicsitem(item));
        if(node)
        {
            auto nodeIn = node->getNodeInputAtPosition(event->screenPos().toPoint());
            if(nodeIn)
            // Open connection was released on a NodeInput
            {
                if(!nodeIn->hasConnection())
                {
                    establishConnection(nodeIn);
                    return;
                }
            }
        }
        destroyOpenConnection();
    }
    if (event->button() == Qt::MiddleButton)
    {
        middleMouseIsDragging = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void NodeGraph::wheelEvent(QWheelEvent* event)
{
    QPoint scrollAmount = event->angleDelta();
    double factor = (scrollAmount.y() > 0) ? 1.2 : 1 / 1.2;
    if (viewScale * factor > 0.1 && viewScale * factor < 10.0)
    {
        this->scale(factor, factor);
        viewScale *= factor;
    }
}
