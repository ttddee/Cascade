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
#include "nodefactory.h"
#include "../log.h"

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

    mModel = new NodeGraphModel(this);

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

NodeGraphModel* NodeGraph::getModel()
{
    return mModel;
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

void NodeGraph::handleNodeLeftClicked(NodeBase* node)
{
    //selectNode(node);
}

void NodeGraph::handleNodeDoubleClicked(NodeBase* node)
{
//    if (node)
//    {
//        activateNode(node);
//    }
//    else
//    {
//        mActiveNode = nullptr;
//    }
}

void NodeGraph::handleNodeOutputLeftClicked(NodeOutput* nodeOut)
{
//    mLeftMouseIsDragging = true;

//    createOpenConnection(nodeOut);
}

void NodeGraph::handleNodeUpdateRequest(NodeBase* node)
{
    if (node->getIsViewed())
    {
        emit requestNodeDisplay(node);
    }
}

void NodeGraph::handleNodeDisplayRequest(NodeBase* node)
{
    //viewNode(node);
}

void NodeGraph::handleFileSaveRequest(
        NodeBase* node,
        const QString& path,
        const QString& fileType,
        const QMap<std::string, std::string>& attributes,
        const bool batchRender)
{
//    if (batchRender)
//    {
//        // Get all upstream Read Nodes
//        std::vector<NodeBase*> upstreamNodes;
//        node->getAllUpstreamNodes(upstreamNodes);

//        std::vector<ReadNode*> readNodes;
//        for (auto& n : mNodes)
//        {
//            if (n->getType() == NodeType::eRead)
//                readNodes.push_back(static_cast<ReadNode*>(n));
//        }

//        // Get how many images we need to render
//        int numImagesToRender = 0;
//        for (auto& n : readNodes)
//        {
//            if (n->getNumImages() > numImagesToRender)
//            {
//                numImagesToRender = n->getNumImages();
//            }
//        }

//        int digits = QString::number(numImagesToRender).length();

//        for (auto& n : readNodes)
//            n->switchToFirstImage();

//        QProgressDialog progress(
//                    "Rendering Images...",
//                    "Cancel",
//                    0,
//                    numImagesToRender,
//                    this);
//        progress.setWindowModality(Qt::WindowModal);
//        progress.setMinimumSize(QSize(350, 100));

//        progress.setValue(0);
//        progress.show();

//        for (int i = 0; i < numImagesToRender; ++i)
//        {
//            progress.setValue(i);
//            QCoreApplication::processEvents();
//            if (progress.wasCanceled())
//                break;

//            bool isLast = false;
//            if (i == numImagesToRender - 1)
//            {
//                isLast = true;
//            }
//            viewNode(node);
//            emit requestNodeFileSave(
//                        node,
//                        path +
//                        "-" +
//                        QString::number(i).rightJustified(digits, '0') +
//                        "." +
//                        fileType,
//                        attributes,
//                        batchRender,
//                        isLast);
//            for (auto& n : readNodes)
//                n->switchToNextImage();
//        }
//    }
//    else
//    {
//        viewNode(node);
//        emit requestNodeFileSave(node, path + "." + fileType, attributes);
//    }
}

void NodeGraph::handleShutdownRequest()
{
    //flushCacheAllNodes();
}

void NodeGraph::handleSwitchToViewerMode(const ViewerMode mode)
{
    //viewNode(mSelectedNode);
}

void NodeGraph::handleConnectedNodeInputClicked(Connection* c)
{
//    mLeftMouseIsDragging = true;
//    auto sourceOut = c->getSourceOutput();
//    createOpenConnection(sourceOut);
//    deleteConnection(c);
}

void NodeGraph::handleDeleteKeyPressed()
{
//    if (auto node = mModel->getSelectedNode())
//    {
//        deleteNode(node);
//    }
}

void NodeGraph::handleCreateStartupProject()
{
    //createProject();
}

void NodeGraph::handleCreateNewProject()
{
    //clearGraph();
    //createProject();
}

void NodeGraph::handleLoadProject(const QJsonArray& jsonNodeGraph)
{
    //loadProject(jsonNodeGraph);
}

void NodeGraph::mousePressEvent(QMouseEvent* event)
{
//    if (event->button() == Qt::LeftButton)
//    {
//        foreach(auto* n, mNodes)
//        {
//            emit requestSetNodeSelected(n, false);
//        }
//        mSelectedNode = nullptr;
//    }
    if (event->button() == Qt::RightButton)
    {
        // Check if we clicked on something, show context menu if not
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
//    if (mMiddleMouseIsDragging)
//    // Scroll the whole scene
//    {
//        auto t = event->pos() - mLastMousePos;
//        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - t.x());
//        verticalScrollBar()->setValue(verticalScrollBar()->value() - t.y());
//    }
//    else if(mLeftMouseIsDragging)
//    {
//        if(mOpenConnection)
//        {
//            mOpenConnection->updatePosition(
//                        QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x() + 5,
//                               mapToScene(mapFromGlobal(QCursor::pos())).y() + 3));
//        }
//    }
//    mLastMousePos = event->pos();

//    QGraphicsView::mouseMoveEvent(event);
}

void NodeGraph::mouseReleaseEvent(QMouseEvent* event)
{
//    if (event->button() == Qt::LeftButton)
//    {
//        mLeftMouseIsDragging = false;

//        auto item = getObjectUnderCursor();

//        NodeBase* node = qobject_cast<NodeBase*>(getWidgetFromGraphicsItem(item));
//        if(node)
//        {
//            if (auto in = node->getOpenInput())
//            {
//                if (mOpenConnection)
//                    establishConnection(in);
//            }
//        }
//        destroyOpenConnection();
//    }
//    if (event->button() == Qt::MiddleButton)
//    {
//        mMiddleMouseIsDragging = false;
//    }

//    QGraphicsView::mouseReleaseEvent(event);
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
