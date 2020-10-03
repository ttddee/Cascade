#include "windowmanager.h"

#include <iostream>

#include <QKeyEvent>

#include "vulkanwindow.h"
#include "nodegraph.h"
#include "propertiesview.h"
#include "viewerstatusbar.h"

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
}

bool WindowManager::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_F1)
        {
            currentViewerMode = VIEWER_MODE_BACK;
        }
        else if (keyEvent->key() == Qt::Key_F2)
        {
            currentViewerMode = VIEWER_MODE_FRONT;
        }
        else if (keyEvent->key() == Qt::Key_F3)
        {
            currentViewerMode = VIEWER_MODE_ALPHA;
        }
        else if (keyEvent->key() == Qt::Key_F4)
        {
            currentViewerMode = VIEWER_MODE_OUTPUT;
            nodeGraph->viewNode(nodeGraph->getSelectedNode());

            return true;
        }
    }

    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            // std::cout << "left button" << std::endl;
        }
    }
    Q_UNUSED(watched);

    return false;
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

