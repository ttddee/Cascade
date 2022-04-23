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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QTimer>
#include <QCloseEvent>

#include "renderer/vulkanrenderer.h"
#include "log.h"
#include "popupmessages.h"
#include "preferencesdialog.h"
#include "aboutdialog.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::DockWidgetArea;

namespace Cascade {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      mUi(new Ui::MainWindow)
{
    // OCIO needs the locale to be set here
    std::setlocale(LC_NUMERIC, "C");
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    mUi->setupUi(this);

    mDockManager = new CDockManager(this);

    mViewerStatusBar = new ViewerStatusBar();

    mVulkanView = new VulkanView(mViewerStatusBar);
    CDockWidget* vulkanViewDockWidget = new CDockWidget("Viewer");
    vulkanViewDockWidget->setWidget(mVulkanView);
    auto* centralDockArea = mDockManager->setCentralWidget(vulkanViewDockWidget);
    centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    mIsfManager = &ISFManager::getInstance();
    mIsfManager->setUp();

    mNodeGraph = new NodeGraph();
    mNodeGraph->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mNodeGraph->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mNodeGraphDockWidget = new CDockWidget("Node Graph");
    mNodeGraphDockWidget->setWidget(mNodeGraph);
    mDockManager->addDockWidget(
                DockWidgetArea::BottomDockWidgetArea,
                mNodeGraphDockWidget,
                centralDockArea);

    mPropertiesView = new PropertiesView();
    mPropertiesViewDockWidget = new CDockWidget("Properties");
    mPropertiesViewDockWidget->setWidget(mPropertiesView);
    mPropertiesViewDockWidget->resize(700, 700);
    mDockManager->addDockWidget(
                DockWidgetArea::RightDockWidgetArea,
                mPropertiesViewDockWidget,
                centralDockArea);

    mMainMenu = new MainMenu(this);
    this->setMenuBar(mMainMenu);

    mWindowManager = &WindowManager::getInstance();
    mWindowManager->setUp(
                mVulkanView->getVulkanWindow(),
                mNodeGraph,
                mPropertiesView,
                mViewerStatusBar);

    mProjectManager = &ProjectManager::getInstance();
    mProjectManager->setUp(mNodeGraph);
    connect(mProjectManager, &ProjectManager::projectTitleChanged,
            this, &MainWindow::handleProjectTitleChanged);

    mPreferencesManager = &PreferencesManager::getInstance();
    mPreferencesManager->setUp();

    // Incoming
    connect(mVulkanView->getVulkanWindow(), &VulkanWindow::rendererHasBeenCreated,
            this, &MainWindow::handleRendererHasBeenCreated);
    connect(mVulkanView->getVulkanWindow(), &VulkanWindow::noGPUFound,
            this, &MainWindow::handleNoGPUFound);
    connect(mVulkanView->getVulkanWindow(), &VulkanWindow::deviceLost,
            this, &MainWindow::handleDeviceLost);

    // Outgoing
    connect(this, &MainWindow::requestShutdown,
            mNodeGraph, &NodeGraph::handleShutdownRequest);

    QSettings::setPath(
                QSettings::IniFormat,
                QSettings::SystemScope,
                QDir::currentPath());

    mProjectManager->createStartupProject();

    // Bring window to front, just in case it isn't
    this->setWindowState(Qt::WindowActive);
}

NodeGraph* MainWindow::getNodeGraph() const
{
    return mNodeGraph;
}

void MainWindow::handleRendererHasBeenCreated()
{
    // We are waiting for the renderer to be fully
    // initialized here before using it
    mRenderManager = &RenderManager::getInstance();
    mRenderManager->setUp(mVulkanView->getVulkanWindow()->getRenderer(), mNodeGraph);

    this->statusBar()->showMessage("GPU: " + mVulkanView->getVulkanWindow()->getRenderer()->getGpuName());
}

void MainWindow::handleNoGPUFound()
{
    executeMessageBox(MESSAGEBOX_NO_GPU_FOUND);

    closeEvent(nullptr);
}

void MainWindow::handleDeviceLost()
{
    executeMessageBox(MESSAGEBOX_DEVICE_LOST);

    closeEvent(nullptr);
}

void MainWindow::handleProjectTitleChanged(const QString& t)
{
    QString spacer = "- ";
    if (t == "*")
        spacer = "";
    mNodeGraphDockWidget->setTitle("Node Graph " + spacer + t);
}

void MainWindow::handleNewProjectAction()
{
    mProjectManager->createNewProject();
}

void MainWindow::handleOpenProjectAction()
{
    mProjectManager->loadProject();
}

void MainWindow::handleSaveProjectAction()
{
    mProjectManager->saveProject();
}

void MainWindow::handleSaveProjectAsAction()
{
    mProjectManager->saveProjectAs();
}

void MainWindow::handleExitAction()
{
    // Using this instead of QApplication::quit(),
    // so that the renderer can shut down gracefully
    closeEvent(nullptr);
}

void MainWindow::handlePreferencesAction()
{
    auto prefs = new PreferencesDialog(this);
    prefs->show();
}

void MainWindow::handleAboutAction()
{
    auto about = new AboutDialog(this);
    about->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit requestShutdown();

    mVulkanView->getVulkanWindow()->getRenderer()->shutdown();

    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete mUi;
}

} // namespace Cascade

