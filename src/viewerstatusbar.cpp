#include "viewerstatusbar.h"
#include "ui_viewerstatusbar.h"

ViewerStatusBar::ViewerStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerStatusBar)
{
    ui->setupUi(this);

    connect(ui->zoomResetButton, &QPushButton::clicked,
            this, &ViewerStatusBar::requestZoomReset);
}

void ViewerStatusBar::setZoomText(const QString &s)
{
    ui->zoomLabel->setText(s);
}

void ViewerStatusBar::setWidthText(const QString &s)
{
    ui->widthLabel->setText(s);
}

void ViewerStatusBar::setHeightText(const QString &s)
{
    ui->heightLabel->setText(s);
}

void ViewerStatusBar::setViewerModeText(const QString &s)
{
    ui->viewerModeLabel->setText(s);
}

ViewerStatusBar::~ViewerStatusBar()
{
    delete ui;
}
