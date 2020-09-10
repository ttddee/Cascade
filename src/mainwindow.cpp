#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    windowManager = &WindowManager::getInstance();
    windowManager->setUp(ui->vulkanView->getVulkanWindow(), ui->nodeGraph, ui->propertiesView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

