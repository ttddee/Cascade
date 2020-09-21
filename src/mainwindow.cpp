#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "vulkanrenderer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
}

void MainWindow::handleRendererHasBeenCreated()
{
    // We are waiting for the renderer to be fully initialized here
    // before using it
    renderManager = &RenderManager::getInstance();
    renderManager->setUp(ui->vulkanView->getVulkanWindow()->getRenderer(), ui->nodeGraph);

    this->statusBar()->showMessage("GPU: " + ui->vulkanView->getVulkanWindow()->getRenderer()->getGpuName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

