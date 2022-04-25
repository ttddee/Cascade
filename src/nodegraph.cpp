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

namespace Cascade {

NodeGraph::NodeGraph(QWidget* parent)
    : QGraphicsView(parent)
{
    mScene = new QGraphicsScene();
    setScene(mScene);

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);
    setSceneRect(0, 0, mViewWidth, mViewHeight);
    centerOn(mViewWidth / 2, mViewHeight / 2);

    mWindowManager = &WindowManager::getInstance();
    mRenderManager = &RenderManager::getInstance();

    mContextMenu = new NodeGraphContextMenu(this);

    connect(this, &NodeGraph::requestNodeDisplay,
            mRenderManager, &RenderManager::handleNodeDisplayRequest);
    connect(this, &NodeGraph::requestNodeFileSave,
            mRenderManager, &RenderManager::handleNodeFileSaveRequest);
    connect(this, &NodeGraph::requestClearScreen,
            mRenderManager, &RenderManager::handleClearScreenRequest);
    connect(this, &NodeGraph::requestClearProperties,
            mWindowManager, &WindowManager::handleClearPropertiesRequest);
}

void NodeGraph::createProject()
{
    createNode(NODE_TYPE_READ, NodeGraphPosition::eCustom, "", QPoint(29600, 29920), false);
    createNode(NODE_TYPE_WRITE, NodeGraphPosition::eCustom, "", QPoint(30200, 29920), false);
}

void NodeGraph::createNode(
        const NodeType type,
        const NodeGraphPosition position,
        const QString& customName,
        const QPoint coords,
        const bool view)
{
    NodeBase* n = new NodeBase(type, this, nullptr, customName);
    mScene->addWidget(n);

    QPoint nodePos;

    if (position == NodeGraphPosition::eCustom)
        nodePos = coords;
    else
        nodePos = getCoordinatesForPosition(position);

    n->move(nodePos);

    mNodes.push_back(n);

    connectNodeSignals(n);

    if (view)
        viewNode(n);

    mLastCreatedNodePos = nodePos;

    emit projectIsDirty();
}

NodeBase* NodeGraph::loadNode(const NodePersistentProperties& p)
{
    NodeBase* n = new NodeBase(p.mNodeType, this, nullptr, p.mCustomName);
    mScene->addWidget(n);
    n->move(p.mPos);
    n->setID(p.mUuid);
    mNodes.push_back(n);

    connectNodeSignals(n);

    n->setInputIDs(p.mInputs);
    n->loadNodePropertyValues(p.mProperties);

    return n;
}

void NodeGraph::connectNodeSignals(NodeBase* n)
{
    // Node to graph
    connect(n, &NodeBase::nodeWasLeftClicked,
                this, &NodeGraph::handleNodeLeftClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            this, &NodeGraph::handleNodeDoubleClicked);
    connect(n, &NodeBase::nodeRequestUpdate,
            this, &NodeGraph::handleNodeUpdateRequest);
    // Graph to node
    connect(this, &NodeGraph::requestSetNodeSelected,
            n, &NodeBase::handleSetSelected);
    connect(this, &NodeGraph::requestSetNodeActive,
            n, &NodeBase::handleSetActive);
    connect(this, &NodeGraph::requestSetNodeViewed,
            n, &NodeBase::handleSetViewed);

    connect(n, &NodeBase::nodeWasDoubleClicked,
            mWindowManager, &WindowManager::handleNodeDoubleClicked);

    if (n->getType() == NODE_TYPE_WRITE)
    {
        connect(n, &NodeBase::nodeRequestFileSave,
                this, &NodeGraph::handleFileSaveRequest);
    }
}

void NodeGraph::loadProject(const QJsonArray& jsonNodeGraph)
{
    QJsonObject jsonNodesHeading = jsonNodeGraph.at(0).toObject();
    QJsonArray jsonNodesArray = jsonNodesHeading.value("nodes").toArray();
    QJsonObject jsonConnectionsHeading = jsonNodeGraph.at(1).toObject();
    QJsonArray jsonConnectionsArray = jsonConnectionsHeading.value("connections").toArray();

    clearGraph();

    for (int i = 0; i < jsonNodesArray.size(); i++)
    {
        QJsonObject jsonNode = jsonNodesArray.at(i).toObject();

        // Set properties to loaded values
        NodePersistentProperties p;
        p.mNodeType = nodeStrings.key(jsonNode["type"].toString());
        p.mPos = QPoint(jsonNode["posx"].toInt(), jsonNode["posy"].toInt());
        p.mUuid = jsonNode["uuid"].toString();
        p.mCustomName = jsonNode["customname"].toString();

        // Get UUID for Node Inputs
        QJsonObject ins = jsonNode["inputs"].toObject();
        for (int i = 0; i < ins.size(); i++)
        {
            p.mInputs[i] = ins.value(QString::number(i)).toString();
        }

        // Load properties to array
        QJsonObject props = jsonNode["properties"].toObject();
        for (int i = 0; i < props.size(); i++)
        {
            p.mProperties[i] = props.value(QString::number(i)).toString();
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
        if (NodeBase* n = findNodeById(src))
        {
            srcNode = n;
            if (NodeBase* n = findNodeById(dstNode))
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

    mNodes.erase(remove(mNodes.begin(), mNodes.end(), node), mNodes.end());

    mScene->removeItem(node->graphicsProxyWidget());

    if (node == mViewedNode)
    {
        emit requestClearScreen();
        mViewedNode = nullptr;
    }
    if (node == mActiveNode)
    {
        emit requestClearProperties();
        mActiveNode = nullptr;
    }

    delete node;

    mSelectedNode = nullptr;

    emit projectIsDirty();
}

float NodeGraph::getViewScale() const
{
    return mViewScale;
}

void NodeGraph::showContextMenu()
{
    mContextMenu->exec(QCursor::pos());
}

QGraphicsItem* NodeGraph::getObjectUnderCursor()
{
    auto item = mScene->itemAt(mapToScene(mapFromGlobal(QCursor::pos())), QTransform());

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

const QPoint NodeGraph::getCoordinatesForPosition(const NodeGraphPosition pos)
{
    if (pos == NodeGraphPosition::eRelativeToLastNode)
    {
        return mLastCreatedNodePos + QPoint(100, 30);
    }
    else if (pos == NodeGraphPosition::eAtCursor)
    {
        return mapToScene(mLastMousePos).toPoint();
    }
    else
    {
        return QPoint(0, 0);
    }
}

void NodeGraph::selectNode(NodeBase *node)
{
    mSelectedNode = node;
    foreach(NodeBase* n, mNodes)
    {
        emit requestSetNodeSelected(n, false);
    }
    emit requestSetNodeSelected(node, true);
}

void NodeGraph::activateNode(NodeBase *node)
{
    mActiveNode = node;
    emit requestSetNodeActive(node, true);
}

NodeBase* NodeGraph::findNodeById(const QString& id)
{
    foreach(NodeBase* n, mNodes)
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
        mViewedNode = node;

        foreach(NodeBase* n, mNodes)
        {
            emit requestSetNodeViewed(n, false);
        }
        emit requestSetNodeViewed(node, true);
        emit requestNodeDisplay(node);
    }
}

NodeBase* NodeGraph::getViewedNode()
{
    return mViewedNode;
}

void NodeGraph::handleNodeCreationRequest(
        const NodeType type,
        const NodeGraphPosition pos,
        const QString& customName)
{
    createNode(type, pos, customName);
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
        mActiveNode = nullptr;
    }
}

void NodeGraph::handleNodeOutputLeftClicked(NodeOutput* nodeOut)
{
    mLeftMouseIsDragging = true;

    createOpenConnection(nodeOut);
}

void NodeGraph::handleNodeUpdateRequest(NodeBase* node)
{
    if (node->isViewed())
    {
        emit requestNodeDisplay(node);
    }
}

void NodeGraph::handleNodeDisplayRequest(NodeBase* node)
{
    viewNode(node);
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
        for (auto& n : mNodes)
        {
            if (n->getType() == NODE_TYPE_READ)
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

void NodeGraph::handleShutdownRequest()
{
    flushCacheAllNodes();
}

void NodeGraph::handleSwitchToViewerMode(const ViewerMode mode)
{
    viewNode(mSelectedNode);
}

Connection* NodeGraph::createOpenConnection(NodeOutput* nodeOut)
{
    Connection* c = new Connection(nodeOut);
    mScene->addItem(c);
    mOpenConnection = c;

    mOpenConnection->updatePosition(
                QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x(),
                       mapToScene(mapFromGlobal(QCursor::pos())).y()));

    return c;
}

void NodeGraph::destroyOpenConnection()
{
    if(mOpenConnection)
    {
        mScene->removeItem(mOpenConnection);
        delete mOpenConnection;
        mOpenConnection = nullptr;
    }
}

void NodeGraph::establishConnection(NodeInput *nodeIn)
{
    mOpenConnection->mTargetInput = nodeIn;
    mOpenConnection->mSourceOutput->addConnection(mOpenConnection);
    mConnections.push_back(mOpenConnection);
    nodeIn->addInConnection(mOpenConnection);
    mOpenConnection = nullptr;

    emit projectIsDirty();
}

void NodeGraph::loadConnection(NodeOutput* src, NodeInput* dst)
{
    Connection* c = new Connection(src);
    mScene->addItem(c);
    c->mTargetInput = dst;
    c->mSourceOutput->addConnection(c);
    mConnections.push_back(c);
    dst->addInConnectionNoUpdate(c);
}

void NodeGraph::deleteConnection(Connection* c)
{
    for(size_t i = 0; i < mConnections.size(); ++i)
    {
        if (c == mConnections[i])
        {
            mConnections.erase(mConnections.begin() + i);
        }
    }

    c->mSourceOutput->removeConnection(c);
    c->mTargetInput->removeInConnection();

    mScene->removeItem(c);
    delete c;
    c = nullptr;

    emit projectIsDirty();
}

NodeBase* NodeGraph::getSelectedNode()
{
    return mSelectedNode;
}

void NodeGraph::handleConnectedNodeInputClicked(Connection* c)
{
    mLeftMouseIsDragging = true;
    auto sourceOut = c->mSourceOutput;
    createOpenConnection(sourceOut);
    deleteConnection(c);
}

void NodeGraph::handleDeleteKeyPressed()
{
    if (auto node = getSelectedNode())
    {
        deleteNode(node);
    }
}

void NodeGraph::handleCreateStartupProject()
{
    createProject();
}

void NodeGraph::handleCreateNewProject()
{
    clearGraph();
    createProject();
}

void NodeGraph::handleLoadProject(const QJsonArray& jsonNodeGraph)
{
    loadProject(jsonNodeGraph);
}

void NodeGraph::getNodeGraphAsJson(QJsonArray& jsonNodeGraph)
{
    QJsonArray jsonNodesArray;

    foreach (const auto& node, mNodes)
    {
        node->addNodeToJsonArray(jsonNodesArray);
    }
    QJsonObject jsonNodesHeading {
        { "nodes", jsonNodesArray }
    };
    jsonNodeGraph.push_back(jsonNodesHeading);

    QJsonArray jsonConnectionsArray;
    foreach (const auto& connection, mConnections)
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
    for (auto& n : mNodes)
        n->flushCache();
}

void NodeGraph::clearGraph()
{
    foreach (const auto& connection, mConnections)
    {
        deleteConnection(connection);
    }
    foreach (const auto& node, mNodes)
    {
        deleteNode(node);
    }
}

void NodeGraph::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        foreach(NodeBase* n, mNodes)
        {
            emit requestSetNodeSelected(n, false);
        }
    }
    if (event->button() == Qt::RightButton)
    {
        // Check if we clicked on something
        // Show context menu if not
        auto item = getWidgetFromGraphicsItem(getObjectUnderCursor());
        if (!item)
        {
            showContextMenu();
        }
    }
    else if (event->button() == Qt::MiddleButton)
    {
        mMiddleMouseIsDragging = true;
    }
    mLastMousePos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void NodeGraph::mouseMoveEvent(QMouseEvent* event)
{
    if (mMiddleMouseIsDragging)
    // Scroll the whole scene
    {
        auto t = event->pos() - mLastMousePos;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - t.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - t.y());
    }
    else if(mLeftMouseIsDragging)
    {
        if(mOpenConnection)
        {
            mOpenConnection->updatePosition(
                        QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x() + 5,
                               mapToScene(mapFromGlobal(QCursor::pos())).y() + 3));
        }
    }
    mLastMousePos = event->pos();

    QGraphicsView::mouseMoveEvent(event);
}

void NodeGraph::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mLeftMouseIsDragging = false;

        auto item = getObjectUnderCursor();

        NodeBase* node = qobject_cast<NodeBase*>(getWidgetFromGraphicsItem(item));
        if(node)
        {
            if (auto in = node->getOpenInput())
            {
                if (mOpenConnection)
                    establishConnection(in);
            }
        }
        destroyOpenConnection();
    }
    if (event->button() == Qt::MiddleButton)
    {
        mMiddleMouseIsDragging = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void NodeGraph::wheelEvent(QWheelEvent* event)
{
    QPoint scrollAmount = event->angleDelta();
    double factor = (scrollAmount.y() > 0) ? 1.2 : 1 / 1.2;
    if (mViewScale * factor > 0.1 && mViewScale * factor < 10.0)
    {
        scale(factor, factor);
        mViewScale *= factor;
    }
}

} // namespace Cascade
