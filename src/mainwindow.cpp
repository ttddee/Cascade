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
      ui(new Ui::MainWindow)
{
    // OCIO needs the locale to be set here
    std::setlocale(LC_NUMERIC, "C");
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    ui->setupUi(this);

    dockManager = new CDockManager(this);

    viewerStatusBar = new ViewerStatusBar();

    vulkanView = new VulkanView(viewerStatusBar);
    CDockWidget* vulkanViewDockWidget = new CDockWidget("Viewer");
    vulkanViewDockWidget->setWidget(vulkanView);
    auto* centralDockArea = dockManager->setCentralWidget(vulkanViewDockWidget);
    centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    isfManager = &ISFManager::getInstance();
    isfManager->setUp();

    nodeGraph = new NodeGraph();
    nodeGraph->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    nodeGraph->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    nodeGraphDockWidget = new CDockWidget("Node Graph");
    nodeGraphDockWidget->setWidget(nodeGraph);
    dockManager->addDockWidget(
                DockWidgetArea::BottomDockWidgetArea,
                nodeGraphDockWidget,
                centralDockArea);

    propertiesView = new PropertiesView();
    propertiesViewDockWidget = new CDockWidget("Properties");
    propertiesViewDockWidget->setWidget(propertiesView);
    propertiesViewDockWidget->resize(700, 700);
    dockManager->addDockWidget(
                DockWidgetArea::RightDockWidgetArea,
                propertiesViewDockWidget,
                centralDockArea);

    mainMenu = new MainMenu(this);
    this->setMenuBar(mainMenu);

    windowManager = &WindowManager::getInstance();
    windowManager->setUp(
                vulkanView->getVulkanWindow(),
                nodeGraph,
                propertiesView,
                viewerStatusBar);

    projectManager = &ProjectManager::getInstance();
    projectManager->setUp(nodeGraph);
    connect(projectManager, &ProjectManager::projectTitleChanged,
            this, &MainWindow::handleProjectTitleChanged);

    preferencesManager = &PreferencesManager::getInstance();
    preferencesManager->setUp();

    connect(vulkanView->getVulkanWindow(), &VulkanWindow::rendererHasBeenCreated,
            this, &MainWindow::handleRendererHasBeenCreated);
    connect(vulkanView->getVulkanWindow(), &VulkanWindow::noGPUFound,
            this, &MainWindow::handleNoGPUFound);
    connect(vulkanView->getVulkanWindow(), &VulkanWindow::deviceLost,
            this, &MainWindow::handleDeviceLost);

    QSettings::setPath(
                QSettings::IniFormat,
                QSettings::SystemScope,
                QDir::currentPath());

    projectManager->createStartupProject();

    // Bring window to front, just in case it isn't
    this->setWindowState(Qt::WindowActive);
}

NodeGraph* MainWindow::getNodeGraph() const
{
    return nodeGraph;
}

void MainWindow::handleRendererHasBeenCreated()
{
    // We are waiting for the renderer to be fully
    // initialized here before using it
    renderManager = &RenderManager::getInstance();
    renderManager->setUp(vulkanView->getVulkanWindow()->getRenderer(), nodeGraph);

    this->statusBar()->showMessage("GPU: " + vulkanView->getVulkanWindow()->getRenderer()->getGpuName());
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
    nodeGraphDockWidget->setTitle("Node Graph " + spacer + t);
}

void MainWindow::handleNewProjectAction()
{
    projectManager->createNewProject();
}

void MainWindow::handleOpenProjectAction()
{
    projectManager->loadProject();
}

void MainWindow::handleSaveProjectAction()
{
    projectManager->saveProject();
}

void MainWindow::handleSaveProjectAsAction()
{
    projectManager->saveProjectAs();
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
    nodeGraph->flushCacheAllNodes();

    vulkanView->getVulkanWindow()->getRenderer()->shutdown();

    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace Cascade

