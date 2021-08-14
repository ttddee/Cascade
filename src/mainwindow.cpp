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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include <QMessageBox>
#include <QComboBox>
#include <QDir>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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
    CDockWidget* nodeGraphDockWidget = new CDockWidget("Node Graph");
    nodeGraphDockWidget->setWidget(nodeGraph);
    dockManager->addDockWidget(
                DockWidgetArea::BottomDockWidgetArea,
                nodeGraphDockWidget,
                centralDockArea);

    propertiesView = new PropertiesView();
    CDockWidget* propertiesViewDockWidget = new CDockWidget("Properties");
    propertiesViewDockWidget->setWidget(propertiesView);
    propertiesViewDockWidget->resize(700, 700);
    dockManager->addDockWidget(
                DockWidgetArea::RightDockWidgetArea,
                propertiesViewDockWidget,
                centralDockArea);

    //--------- Main Menu

    newProjectAction = new QAction("New Project");
    ui->menuFile->addAction(newProjectAction);

    ui->menuFile->addSeparator();

    openProjectAction = new QAction("Open Project");
    ui->menuFile->addAction(openProjectAction);

    saveProjectAction = new QAction("Save Project");
    ui->menuFile->addAction(saveProjectAction);

    saveProjectAsAction = new QAction("Save Project As");
    ui->menuFile->addAction(saveProjectAsAction);

    ui->menuFile->addSeparator();

    exitAction = new QAction("Exit");
    ui->menuFile->addAction(exitAction);

    ui->menuView->addAction(nodeGraphDockWidget->toggleViewAction());
    ui->menuView->addAction(propertiesViewDockWidget->toggleViewAction());

    ui->menuView->addSeparator();

    // Disabled for now until this is figured out:
    // https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System/issues/272
    saveLayoutAction = new QAction("Save Window Layout");
    //ui->menuView->addAction(saveLayoutAction);
    restoreLayoutAction = new QAction("Restore Saved Layout");
    //ui->menuView->addAction(restoreLayoutAction);

    //ui->menuView->addSeparator();

    restoreDefaultLayoutAction = new QAction("Restore Default Layout");
    //ui->menuView->addAction(restoreDefaultLayoutAction);

    shortcutsAction = new QAction("Shortcuts");
    ui->menuHelp->addAction(shortcutsAction);

    connect(saveProjectAsAction, &QAction::triggered,
            this, &MainWindow::saveProjectAs);
    connect(exitAction, &QAction::triggered,
            QApplication::instance(), QCoreApplication::quit, Qt::QueuedConnection);
    connect(saveLayoutAction, &QAction::triggered,
            this, &MainWindow::saveUserLayout);
    connect(restoreLayoutAction, &QAction::triggered,
            this, &MainWindow::restoreUserLayout);
    connect(restoreDefaultLayoutAction, &QAction::triggered,
            this, &MainWindow::restoreDefaultLayout);
    connect(shortcutsAction, &QAction::triggered,
            this, &MainWindow::displayShortcuts);

    //--------- End Main Menu

    windowManager = &WindowManager::getInstance();
    windowManager->setUp(
                vulkanView->getVulkanWindow(),
                nodeGraph,
                propertiesView,
                viewerStatusBar);

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

void MainWindow::saveProjectAs()
{
    // get path
    QString path;

    QFileDialog dialog(nullptr);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("CSC Project (*.csc)"));
    dialog.setDefaultSuffix("csc");
    auto f = dialog.getSaveFileName();
    CS_LOG_CONSOLE(f);
    if (f.isEmpty())
    {
        return;
    }
    else
    {
        path = f;
    }

    // check path
    QFile jsonFile = QFile(path);
    if (jsonFile.exists())
    {
        // file exists
    }

    // get json from nodegraph
    QJsonDocument project;
    QJsonArray arr;
    QJsonObject info;
    info.insert("Version",
                QString("Cascade Image Editor - v%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
    arr << info;
    nodeGraph->getNodeGraphAsJson(arr);
    project.setArray(arr);


    // save to disk
    //QFile jsonFile("test.csc");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(project.toJson());
}

void MainWindow::saveUserLayout()
{
    QSettings settings(
                QSettings::IniFormat,
                QSettings::SystemScope,
                "settings",
                "user");
    dockManager->addPerspective("custom");
    dockManager->savePerspectives(settings);
}

void MainWindow::restoreUserLayout()
{
    QSettings settings(
                QSettings::IniFormat,
                QSettings::SystemScope,
                "settings",
                "user");
    dockManager->loadPerspectives(settings);
    dockManager->openPerspective("custom");
}

void MainWindow::restoreDefaultLayout()
{

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

MainWindow::~MainWindow()
{
    CS_LOG_INFO("Shutting down");
    CS_LOG_CONSOLE("Shutting down");

    delete ui;

    delete newProjectAction;
    delete openProjectAction;
    delete saveProjectAction;
    delete saveProjectAsAction;
    delete exitAction;
    delete saveLayoutAction;
    delete restoreLayoutAction;
    delete restoreDefaultLayoutAction;
    delete shortcutsAction;

    vulkanView->getVulkanWindow()->getRenderer()->cleanup();
}

