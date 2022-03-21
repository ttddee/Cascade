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

#include "nodegraph.h"

#include <iostream>

#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsProxyWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QProgressDialog>
#include <QApplication>

#include "nodeinput.h"
#include "nodeoutput.h"
#include "log.h"

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
            rManager, &RenderManager::handleClearScreenRequest);
    connect(this, &NodeGraph::requestClearProperties,
            wManager, &WindowManager::handleClearPropertiesRequest);
}

void NodeGraph::createProject()
{
    createNode(NODE_TYPE_READ, QPoint(29600, 29920), false);
    createNode(NODE_TYPE_WRITE, QPoint(30200, 29920), false);
}

void NodeGraph::createNode(
        const NodeType type,
        const QPoint pos,
        const bool view)
{
    NodeBase* n = new NodeBase(type, this);
    scene->addWidget(n);
    n->move(pos);
    nodes.push_back(n);

    connectNodeSignals(n);

    if (view)
        viewNode(n);

    lastCreatedNodePos = pos;

    emit projectIsDirty();
}

NodeBase* NodeGraph::loadNode(const NodePersistentProperties& p)
{
    NodeBase* n = new NodeBase(p.nodeType, this);
    scene->addWidget(n);
    n->move(p.pos);
    n->setID(p.uuid);
    nodes.push_back(n);

    connectNodeSignals(n);

    n->setInputIDs(p.inputs);
    n->loadNodePropertyValues(p.properties);

    return n;
}

void NodeGraph::connectNodeSignals(NodeBase* n)
{
    connect(n, &NodeBase::nodeWasLeftClicked,
                this, &NodeGraph::handleNodeLeftClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            this, &NodeGraph::handleNodeDoubleClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            wManager, &WindowManager::handleNodeDoubleClicked);
    connect(n, &NodeBase::nodeRequestUpdate,
            this, &NodeGraph::handleNodeUpdateRequest);
    if (n->nodeType == NODE_TYPE_WRITE)
    {
        connect(n, &NodeBase::nodeRequestFileSave,
                this, &NodeGraph::handleFileSaveRequest);
    }
}

void NodeGraph::loadProject(
        const QJsonArray& jsonNodesArray,
        const QJsonArray& jsonConnectionsArray)
{
    for (int i = 0; i < jsonNodesArray.size(); i++)
    {
        QJsonObject jsonNode = jsonNodesArray.at(i).toObject();

        // Set properties to loaded values
        NodePersistentProperties p;
        p.nodeType = nodeStrings.key(jsonNode["type"].toString());
        p.pos = QPoint(jsonNode["posx"].toInt(), jsonNode["posy"].toInt());
        p.uuid = jsonNode["uuid"].toString();

        // Get UUID for Node Inputs
        QJsonObject ins = jsonNode["inputs"].toObject();
        for (int i = 0; i < ins.size(); i++)
        {
            p.inputs[i] = ins.value(QString::number(i)).toString();
        }

        // Load properties to array
        QJsonObject props = jsonNode["properties"].toObject();
        for (int i = 0; i < props.size(); i++)
        {
            p.properties[i] = props.value(QString::number(i)).toString();
        }

        loadNode(p);
    }

    for (int i = 0; i < jsonConnectionsArray.size(); i++)
    {
        auto src = jsonConnectionsArray.at(i)["src"].toString();
        auto dst = jsonConnectionsArray.at(i)["dst"].toString();
        auto dstNode = jsonConnectionsArray.at(i)["dst-node"].toString();

        NodeBase* srcNode = nullptr;
        NodeInput* targetInput = nullptr;
        bool found = false;
        if (NodeBase* n = findNode(src))
        {
            srcNode = n;
            if (NodeBase* n = findNode(dstNode))
            {

                if (NodeInput* in = n->findNodeInput(dst))
                {
                    targetInput = in;
                    loadConnection(srcNode->getRgbaOut(), targetInput);
                    found = true;
                }
            }
        }
        if (!found)
            CS_LOG_WARNING("Could not load connection.");
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
    if (node == selectedNode)
    {
        emit requestClearProperties();
    }

    delete node;

    selectedNode = nullptr;

    emit projectIsDirty();
}

float NodeGraph::getViewScale() const
{
    return viewScale;
}

void NodeGraph::showContextMenu()
{
    contextMenu->exec(QCursor::pos());
}

QGraphicsItem* NodeGraph::getObjectUnderCursor()
{
    auto item = scene->itemAt(mapToScene(mapFromGlobal(QCursor::pos())), QTransform());

    return item;
}

QWidget* NodeGraph::getWidgetFromGraphicsItem(QGraphicsItem *item)
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

NodeBase* NodeGraph::findNode(const QString& id)
{
    foreach(NodeBase* n, nodes)
    {
        if (n->getID() == id)
            return n;
    }
    return nullptr;
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
        emit requestNodeDisplay(node);
    }
}

void NodeGraph::handleFileSaveRequest(
        NodeBase* node,
        const QString& path,
        const QString& fileType,
        const QMap<std::string, std::string>& attributes,
        const bool batchRender)
{
    if (batchRender)
    {
        // Get all upstream Read Nodes
        std::vector<NodeBase*> upstreamNodes;
        node->getAllUpstreamNodes(upstreamNodes);

        std::vector<NodeBase*> readNodes;
        for (auto& n : nodes)
        {
            if (n->nodeType == NODE_TYPE_READ)
                readNodes.push_back(n);
        }

        // Get how many images we need to render
        int numImagesToRender = 0;
        for (auto& n : readNodes)
        {
            if (n->getNumImages() > numImagesToRender)
            {
                numImagesToRender = n->getNumImages();
            }
        }

        int digits = QString::number(numImagesToRender).length();

        for (auto& n : readNodes)
            n->switchToFirstImage();

        QProgressDialog progress(
                    "Rendering Images...",
                    "Cancel",
                    0,
                    numImagesToRender,
                    this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumSize(QSize(350, 100));

        progress.setValue(0);
        progress.show();

        for (int i = 0; i < numImagesToRender; ++i)
        {
            progress.setValue(i);
            QCoreApplication::processEvents();
            if (progress.wasCanceled())
                break;

            bool isLast = false;
            if (i == numImagesToRender - 1)
            {
                isLast = true;
            }
            viewNode(node);
            emit requestNodeFileSave(
                        node,
                        path +
                        "-" +
                        QString::number(i).rightJustified(digits, '0') +
                        "." +
                        fileType,
                        attributes,
                        batchRender,
                        isLast);
            for (auto& n : readNodes)
                n->switchToNextImage();
        }
    }
    else
    {
        viewNode(node);
        emit requestNodeFileSave(node, path + "." + fileType, attributes);
    }
}

Connection* NodeGraph::createOpenConnection(NodeOutput* nodeOut)
{
    Connection* c = new Connection(nodeOut);
    scene->addItem(c);
    openConnection = c;

    openConnection->updatePosition(
                QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x(),
                       mapToScene(mapFromGlobal(QCursor::pos())).y()));

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

    emit projectIsDirty();
}

void NodeGraph::loadConnection(NodeOutput* src, NodeInput* dst)
{
    Connection* c = new Connection(src);
    scene->addItem(c);
    c->targetInput = dst;
    c->sourceOutput->addConnection(c);
    connections.push_back(c);
    dst->addInConnectionNoUpdate(c);
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

    emit projectIsDirty();
}

NodeBase* NodeGraph::getSelectedNode()
{
    return selectedNode;
}

void NodeGraph::handleConnectedNodeInputClicked(Connection* c)
{
    leftMouseIsDragging = true;
    auto sourceOut = c->sourceOutput;
    createOpenConnection(sourceOut);
    deleteConnection(c);
}

void NodeGraph::getNodeGraphAsJson(QJsonArray& jsonNodeGraph)
{
    QJsonArray jsonNodesArray;

    foreach (const auto& node, nodes)
    {
        node->addNodeToJsonArray(jsonNodesArray);
    }
    QJsonObject jsonNodesHeading {
        { "nodes", jsonNodesArray }
    };
    jsonNodeGraph.push_back(jsonNodesHeading);

    QJsonArray jsonConnectionsArray;
    foreach (const auto& connection, connections)
    {
        connection->addConnectionToJsonObject(jsonConnectionsArray);
    }
    QJsonObject jsonConnectionsHeading {
        { "connections", jsonConnectionsArray }
    };
    jsonNodeGraph.push_back(jsonConnectionsHeading);
}

void NodeGraph::flushCacheAllNodes()
{
    for (auto& n : nodes)
        n->flushCache();
}

void NodeGraph::clear()
{
    foreach (const auto& connection, connections)
    {
        this->deleteConnection(connection);
    }
    foreach (const auto& node, nodes)
    {
        this->deleteNode(node);
    }
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
        showContextMenu();
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
            openConnection->updatePosition(
                        QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x() + 5,
                               mapToScene(mapFromGlobal(QCursor::pos())).y() + 3));
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

        NodeBase* node = qobject_cast<NodeBase*>(getWidgetFromGraphicsItem(item));
        if(node)
        {
            if (auto in = node->getOpenInput())
            {
                if (openConnection)
                    establishConnection(in);
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
