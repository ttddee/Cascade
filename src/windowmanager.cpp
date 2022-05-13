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
#include "nodegraph/nodegraph.h"
#include "nodegraph/nodebase.h"
#include "propertiesview.h"
#include "viewerstatusbar.h"
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
    mVulkanWindow = vw;
    mNodeGraph = ng;
    mPropertiesView = pv;
    mViewerStatusBar = vb;

    mVulkanWindow->installEventFilter(this);
    mNodeGraph->installEventFilter(this);
    mPropertiesView->installEventFilter(this);
    mViewerStatusBar->installEventFilter(this);

    // Incoming
    connect(mVulkanWindow, &VulkanWindow::requestZoomTextUpdate,
            this, &WindowManager::handleZoomTextUpdateRequest);
    connect(mViewerStatusBar, &ViewerStatusBar::requestZoomReset,
            mVulkanWindow, &VulkanWindow::handleZoomResetRequest);
    connect(mVulkanWindow, &VulkanWindow::renderTargetHasBeenCreated,
            this, &WindowManager::handleRenderTargetCreated);
    connect(mViewerStatusBar, &ViewerStatusBar::valueChanged,
            this, &WindowManager::handleViewerStatusBarValueChanged);
    connect(mViewerStatusBar, &ViewerStatusBar::viewerModeChanged,
            this, &WindowManager::handleViewerModeChanged);

    // Outgoing
    connect(this, &WindowManager::deleteKeyPressed,
            mNodeGraph, &NodeGraph::handleDeleteKeyPressed);
    connect(this, &WindowManager::switchToViewerMode,
            mNodeGraph, &NodeGraph::handleSwitchToViewerMode);
    connect(this, &WindowManager::switchToViewerMode,
            mViewerStatusBar, &ViewerStatusBar::handleSwitchToViewerMode);

    mRenderManager = &RenderManager::getInstance();
}

ViewerMode WindowManager::getViewerMode()
{
    return mCurrentViewerMode;
}

bool WindowManager::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_F1)
        {
            setViewerMode(ViewerMode::eFrontRgb);
        }
        else if (keyEvent->key() == Qt::Key_F2)
        {
            setViewerMode(ViewerMode::eBackRgb);
        }
        else if (keyEvent->key() == Qt::Key_F3)
        {
            setViewerMode(ViewerMode::eInputAlpha);
        }
        else if (keyEvent->key() == Qt::Key_F4)
        {
            if (mNodeGraph->getSelectedNode() == mNodeGraph->getViewedNode() &&
                mCurrentViewerMode == ViewerMode::eOutputRgb)
            {
                setViewerMode(ViewerMode::eOutputAlpha);
            }
            else
            {
                setViewerMode(ViewerMode::eOutputRgb);
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
    mCurrentViewerMode = mode;

    emit switchToViewerMode(mode);
}

void WindowManager::handleClearPropertiesRequest()
{
    mPropertiesView->clear();
}

void WindowManager::handleNodeDoubleClicked(NodeBase* node)
{
    mPropertiesView->loadProperties(node->getProperties());
}

void WindowManager::handleZoomTextUpdateRequest(float f)
{
    mViewerStatusBar->setZoomText(QString::number(static_cast<int>(f * 100)));
}

void WindowManager::handleRenderTargetCreated(int w, int h)
{
    mViewerStatusBar->setWidthText(QString::number(w));
    mViewerStatusBar->setHeightText(QString::number(h));
}

void WindowManager::handleViewerModeChanged(const Cascade::ViewerMode mode)
{
    setViewerMode(mode);
}

void WindowManager::handleViewerStatusBarValueChanged()
{
    mRenderManager->updateViewerPushConstants(mViewerStatusBar->getViewerSettings());

    auto node = mNodeGraph->getViewedNode();
    if (node)
    {
        emit mNodeGraph->requestNodeDisplay(node);
    }
}

} // namespace Cascade
