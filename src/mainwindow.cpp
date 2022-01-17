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

#include <iostream>

#include <QMessageBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>

#include "renderer/vulkanrenderer.h"
#include "csmessagebox.h"
#include "log.h"

using namespace ads;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dockManager = new CDockManager(this);

    viewerStatusBar = new ViewerStatusBar();

    vulkanView = new VulkanView(viewerStatusBar);
    CDockWidget* vulkanViewDockWidget = new CDockWidget("Viewer");
    vulkanViewDockWidget->setWidget(vulkanView);
    auto* centralDockArea = dockManager->setCentralWidget(vulkanViewDockWidget);
    centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);


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
    QMessageBox messageBox;
    messageBox.critical(0,"Error","No compatible GPU found. Shutting down!");
    messageBox.setFixedSize(500, 200);

    QApplication::quit();
}

void MainWindow::handleDeviceLost()
{
    QMessageBox messageBox;
    messageBox.critical(0,"Device Lost","We lost contact to the GPU and need to shut down. Sorry!");
    messageBox.setFixedSize(500, 200);

    QApplication::quit();
}

void MainWindow::handleProjectTitleChanged(const QString& t)
{
    QString spacer = "- ";
    if (t == "*")
        spacer = "";
    nodeGraphDockWidget->setTitle("Node Graph " + spacer + t);
}

void MainWindow::displayShortcuts()
{
    CsMessageBox messageBox;
    messageBox.setMinimumSize(600, 400);
    messageBox.setWindowTitle("Shortcuts");
    messageBox.setText("\n"
                       "F1 - View selected node front input\n"
                       "\n"
                       "F2 - View selected node back input\n"
                       "\n"
                       "F3 - View selected node alpha input\n"
                       "\n"
                       "F4 - Toggle between selected node RGB output and alpha output\n"
                       "\n"
                       "Delete - Delete selected node\n"
                       "\n"
                       "Ctrl + Left Click - Reset slider to default\n"
                       );
    messageBox.exec();
}

void MainWindow::handleNewProjectAction()
{
    CS_LOG_CONSOLE("Beep");
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
    QCoreApplication::quit();
}

void MainWindow::handleShortcutsAction()
{
    displayShortcuts();
}

MainWindow::~MainWindow()
{
    CS_LOG_INFO("Shutting down");
    CS_LOG_CONSOLE("Shutting down");

    delete ui;

    vulkanView->getVulkanWindow()->getRenderer()->cleanup();
}

