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

MainWindow::~MainWindow()
{
    delete ui;
}

