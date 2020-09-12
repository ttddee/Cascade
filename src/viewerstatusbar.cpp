#include "viewerstatusbar.h"
#include "ui_viewerstatusbar.h"

ViewerStatusBar::ViewerStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerStatusBar)
{
    ui->setupUi(this);
}

ViewerStatusBar::~ViewerStatusBar()
{
    delete ui;
}
