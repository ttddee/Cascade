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
