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

#include <iostream>

#include "viewerstatusbar.h"
#include "ui_viewerstatusbar.h"

ViewerStatusBar::ViewerStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerStatusBar)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground);

    gammaSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                this);
    gammaSlider->setName("Gamma");
    gammaSlider->setMaximumWidth(250);
    gammaSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(12, gammaSlider);

    gainSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                this);
    gainSlider->setName("Gain");
    gainSlider->setMaximumWidth(250);
    gainSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(13, gainSlider);

    connect(ui->zoomResetButton, &QPushButton::clicked,
            this, &ViewerStatusBar::requestZoomReset);
    connect(ui->bwCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleBwToggled);
    connect(gammaSlider, &CsSliderBoxEntity::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
    connect(gainSlider, &CsSliderBoxEntity::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
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

void ViewerStatusBar::handleBwToggled()
{
    if(!bw)
        bw = true;
    else
        bw = false;
    emit valueChanged();
}

void ViewerStatusBar::handleValueChanged()
{
    emit valueChanged();
}

QString ViewerStatusBar::getViewerSettings()
{
    QString s;
    s.append(QString::number(static_cast<float>(bw)));
    s.append(",");
    s.append(gammaSlider->getValuesAsString());
    s.append(",");
    s.append(gainSlider->getValuesAsString());

    return s;
}

ViewerStatusBar::~ViewerStatusBar()
{
    delete ui;
}
