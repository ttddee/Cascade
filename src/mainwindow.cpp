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

#include "vulkanrenderer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    windowManager = &WindowManager::getInstance();
    windowManager->setUp(
                ui->vulkanView->getVulkanWindow(),
                ui->nodeGraph,
                ui->propertiesView,
                ui->viewerStatusBar);

    connect(ui->vulkanView->getVulkanWindow(), &VulkanWindow::rendererHasBeenCreated,
            this, &MainWindow::handleRendererHasBeenCreated);
    connect(ui->vulkanView->getVulkanWindow(), &VulkanWindow::noGPUFound,
            this, &MainWindow::handleNoGPUFound);
    connect(ui->vulkanView->getVulkanWindow(), &VulkanWindow::deviceLost,
            this, &MainWindow::handleDeviceLost);
}

void MainWindow::handleRendererHasBeenCreated()
{
    // We are waiting for the renderer to be fully
    // initialized here before using it
    renderManager = &RenderManager::getInstance();
    renderManager->setUp(ui->vulkanView->getVulkanWindow()->getRenderer(), ui->nodeGraph);

    this->statusBar()->showMessage("GPU: " + ui->vulkanView->getVulkanWindow()->getRenderer()->getGpuName());
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

MainWindow::~MainWindow()
{
    delete ui;
}

