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

#include "windowmanager.h"

#include <QKeyEvent>

#include "vulkanwindow.h"
#include "nodegraph.h"
#include "propertiesview.h"
#include "viewerstatusbar.h"
#include "nodebase.h"
#include "rendermanager.h"

namespace Cascade {

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

    // Incoming
    connect(vulkanWindow, &VulkanWindow::requestZoomTextUpdate,
            this, &WindowManager::handleZoomTextUpdateRequest);
    connect(viewerStatusBar, &ViewerStatusBar::requestZoomReset,
            vulkanWindow, &VulkanWindow::handleZoomResetRequest);
    connect(vulkanWindow, &VulkanWindow::renderTargetHasBeenCreated,
            this, &WindowManager::handleRenderTargetCreated);
    connect(viewerStatusBar, &ViewerStatusBar::valueChanged,
            this, &WindowManager::handleViewerStatusBarValueChanged);
    connect(viewerStatusBar, &ViewerStatusBar::viewerModeChanged,
            this, &WindowManager::handleViewerModeChanged);

    // Outgoing
    connect(this, &WindowManager::deleteKeyPressed,
            nodeGraph, &NodeGraph::handleDeleteKeyPressed);
    connect(this, &WindowManager::switchToViewerMode,
            nodeGraph, &NodeGraph::handleSwitchToViewerMode);
    connect(this, &WindowManager::switchToViewerMode,
            viewerStatusBar, &ViewerStatusBar::handleSwitchToViewerMode);

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
            setViewerMode(VIEWER_MODE_FRONT_RGB);
        }
        else if (keyEvent->key() == Qt::Key_F2)
        {
            setViewerMode(VIEWER_MODE_BACK_RGB);
        }
        else if (keyEvent->key() == Qt::Key_F3)
        {
            setViewerMode(VIEWER_MODE_INPUT_ALPHA);
        }
        else if (keyEvent->key() == Qt::Key_F4)
        {
            if (nodeGraph->getSelectedNode() == nodeGraph->getViewedNode() &&
                currentViewerMode == VIEWER_MODE_OUTPUT_RGB)
            {
                setViewerMode(VIEWER_MODE_OUTPUT_ALPHA);
            }
            else
            {
                setViewerMode(VIEWER_MODE_OUTPUT_RGB);
            }
        }
        else if (keyEvent->key() == Qt::Key_Delete)
        {
            emit deleteKeyPressed();
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

void WindowManager::setViewerMode(const ViewerMode mode)
{
    currentViewerMode = mode;

    emit switchToViewerMode(mode);
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

void WindowManager::handleViewerModeChanged(const Cascade::ViewerMode mode)
{
    setViewerMode(mode);
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

} // namespace Cascade
