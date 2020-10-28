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
    connect(ui->bwCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleBwToggled);
    connect(ui->gammaSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ViewerStatusBar::handleGammaBoxValueChanged);
    connect(ui->gammaSlider, &QSlider::valueChanged,
            this, &ViewerStatusBar::handleGammaSliderValueChanged);
    connect(ui->gainSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ViewerStatusBar::handleGainBoxValueChanged);
    connect(ui->gainSlider, &QSlider::valueChanged,
            this, &ViewerStatusBar::handleGainSliderValueChanged);
    connect(ui->colorResetButton, &QPushButton::clicked,
            this, &ViewerStatusBar::handleColorResetButtonClicked);
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

void ViewerStatusBar::handleGammaBoxValueChanged(double value)
{
    ui->gammaSlider->blockSignals(true);
    ui->gammaSlider->setValue(value * 100);
    ui->gammaSlider->blockSignals(false);
    emit valueChanged();
}

void ViewerStatusBar::handleGammaSliderValueChanged(int value)
{
    ui->gammaSpinBox->blockSignals(true);
    if (value > 0)
        ui->gammaSpinBox->setValue(value / 100.0);
    else
        ui->gammaSpinBox->setValue(0);
    ui->gammaSpinBox->blockSignals(false);
    emit valueChanged();
}

void ViewerStatusBar::handleGainBoxValueChanged(double value)
{
    ui->gainSlider->blockSignals(true);
    ui->gainSlider->setValue(value * 100);
    ui->gainSlider->blockSignals(false);
    emit valueChanged();
}

void ViewerStatusBar::handleGainSliderValueChanged(int value)
{
    ui->gainSpinBox->blockSignals(true);
    if (value > 0)
        ui->gainSpinBox->setValue(value / 100.0);
    else
        ui->gainSpinBox->setValue(0);
    ui->gainSpinBox->blockSignals(false);
    emit valueChanged();
}

void ViewerStatusBar::handleColorResetButtonClicked()
{
    ui->gainSpinBox->setValue(1.0);
    ui->gammaSpinBox->setValue(1.0);
}

QString ViewerStatusBar::getViewerSettings()
{
    QString s;
    s.append(QString::number(static_cast<float>(bw)));
    s.append(",");
    s.append(QString::number(ui->gammaSpinBox->value()));
    s.append(",");
    s.append(QString::number(ui->gainSpinBox->value()));

    return s;
}

ViewerStatusBar::~ViewerStatusBar()
{
    delete ui;
}
