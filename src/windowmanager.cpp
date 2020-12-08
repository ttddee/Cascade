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

#include "windowmanager.h"

#include <iostream>

#include <QKeyEvent>

#include "vulkanwindow.h"
#include "nodegraph.h"
#include "propertiesview.h"
#include "viewerstatusbar.h"
#include "nodebase.h"
#include "rendermanager.h"

WindowManager& WindowManager::getInstance()
{
    static WindowManager instance;

    return instance;
}

void WindowManager::setUp(
        VulkanWindow *vw,
        NodeGraph *ng,
        PropertiesView *pv,
        ViewerStatusBar* vb)
{
    vulkanWindow = vw;
    nodeGraph = ng;
    propertiesView = pv;
    viewerStatusBar = vb;

    vulkanWindow->installEventFilter(this);
    nodeGraph->installEventFilter(this);
    propertiesView->installEventFilter(this);
    viewerStatusBar->installEventFilter(this);

    connect(vulkanWindow, &VulkanWindow::requestZoomTextUpdate,
            this, &WindowManager::handleZoomTextUpdateRequest);
    connect(viewerStatusBar, &ViewerStatusBar::requestZoomReset,
            vulkanWindow, &VulkanWindow::handleZoomResetRequest);
    connect(vulkanWindow, &VulkanWindow::renderTargetHasBeenCreated,
            this, &WindowManager::handleRenderTargetCreated);
    connect(viewerStatusBar, &ViewerStatusBar::valueChanged,
            this, &WindowManager::handleViewerStatusBarValueChanged);

    rManager = &RenderManager::getInstance();
}

ViewerMode WindowManager::getViewerMode()
{
    return currentViewerMode;
}

bool WindowManager::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_F1)
        {
            currentViewerMode = VIEWER_MODE_FRONT_RGB;
            viewerStatusBar->setViewerModeText("Front RGB");
            vulkanWindow->setViewerMode(currentViewerMode);
            nodeGraph->viewNode(nodeGraph->getSelectedNode());
            viewOutputAlpha = false;
        }
        else if (keyEvent->key() == Qt::Key_F2)
        {
            currentViewerMode = VIEWER_MODE_BACK_RGB;
            viewerStatusBar->setViewerModeText("Back RGB");
            vulkanWindow->setViewerMode(currentViewerMode);
            nodeGraph->viewNode(nodeGraph->getSelectedNode());
            viewOutputAlpha = false;
        }
        else if (keyEvent->key() == Qt::Key_F3)
        {
            currentViewerMode = VIEWER_MODE_INPUT_ALPHA;
            viewerStatusBar->setViewerModeText("Input Mask");
            vulkanWindow->setViewerMode(currentViewerMode);
            nodeGraph->viewNode(nodeGraph->getSelectedNode());
            viewOutputAlpha = false;
        }
        else if (keyEvent->key() == Qt::Key_F4)
        {
            if (nodeGraph->getSelectedNode() != nodeGraph->getViewedNode())
            {
                viewOutputAlpha = false;
            }
            if (!viewOutputAlpha)
            {
                currentViewerMode = VIEWER_MODE_OUTPUT_RGB;
                viewerStatusBar->setViewerModeText("Output RGB");
                viewOutputAlpha = true;
            }
            else
            {
                currentViewerMode = VIEWER_MODE_OUTPUT_ALPHA;
                viewerStatusBar->setViewerModeText("Output Alpha");
                viewOutputAlpha = false;
            }

            vulkanWindow->setViewerMode(currentViewerMode);

            nodeGraph->viewNode(nodeGraph->getSelectedNode());
        }

        if (keyEvent->key() == Qt::Key_Delete)
        {
            nodeGraph->deleteNode(nodeGraph->getSelectedNode());
        }
        return true;
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            return false;
        }
    }
    Q_UNUSED(watched);

    return false;
}

void WindowManager::handleClearPropertiesRequest()
{
    propertiesView->clear();
}

void WindowManager::handleNodeDoubleClicked(NodeBase* node)
{
    propertiesView->loadProperties(node->getProperties());
}

void WindowManager::handleZoomTextUpdateRequest(float f)
{
    viewerStatusBar->setZoomText(QString::number(static_cast<int>(f * 100)));
}

void WindowManager::handleRenderTargetCreated(int w, int h)
{
    viewerStatusBar->setWidthText(QString::number(w));
    viewerStatusBar->setHeightText(QString::number(h));
}

void WindowManager::handleViewerStatusBarValueChanged()
{
    rManager->updateViewerPushConstants(viewerStatusBar->getViewerSettings());

    auto node = nodeGraph->getViewedNode();
    if (node)
    {
        emit nodeGraph->requestNodeDisplay(node);
    }
}


