/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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

namespace Cascade {

ViewerStatusBar::ViewerStatusBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewerStatusBar)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground);

    QMapIterator<ViewerMode, QString> i(viewerModeText);
    while (i.hasNext())
    {
        i.next();
        ui->viewerModeBox->addItem(i.value());
    }
    ui->viewerModeBox->setCurrentIndex(3);
    ui->viewerModeBox->setMinimumWidth(90);

    splitSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                this);
    splitSlider->setName("Split");
    splitSlider->setMaximumWidth(250);
    splitSlider->setMinMaxStepValue(0.0, 1.0, 0.01, 0.5);
    ui->horizontalLayout->insertWidget(11, splitSlider);

    gammaSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                this);
    gammaSlider->setName("Gamma");
    gammaSlider->setMaximumWidth(250);
    gammaSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(15, gammaSlider);

    gainSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                this);
    gainSlider->setName("Gain");
    gainSlider->setMaximumWidth(250);
    gainSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(16, gainSlider);

    connect(ui->zoomResetButton, &QPushButton::clicked,
            this, &ViewerStatusBar::requestZoomReset);
    connect(ui->splitCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleSplitToggled);
    connect(ui->bwCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleBwToggled);
    connect(gammaSlider, &CsSliderBoxEntity::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
    connect(gainSlider, &CsSliderBoxEntity::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
    connect(splitSlider, &CsSliderBoxEntity::valueChanged,
            this, &ViewerStatusBar::handleSplitSliderChanged);
    connect(ui->viewerModeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ViewerStatusBar::handleViewerModeCheckBoxChanged);
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

void ViewerStatusBar::setViewerMode(const ViewerMode m)
{
    ui->viewerModeBox->setCurrentIndex(m);
}

void ViewerStatusBar::handleSplitToggled()
{
    if(!split)
        split = true;
    else
        split = false;
    emit valueChanged();
}

void ViewerStatusBar::handleSplitSliderChanged()
{
    if (split)
    {
        emit valueChanged();
    }
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

void ViewerStatusBar::handleViewerModeCheckBoxChanged()
{
    currentViewerMode = viewerModeText.key(ui->viewerModeBox->currentText());
    emit viewerModeChanged(currentViewerMode);
    ui->viewerModeBox->clearFocus();

    emit valueChanged();
}

QString ViewerStatusBar::getViewerSettings()
{
    // Only use set split viewer on RGB Out or Alpha Out view
    bool viewerSplit = split;
    if (currentViewerMode == VIEWER_MODE_BACK_RGB ||
        currentViewerMode == VIEWER_MODE_FRONT_RGB ||
        currentViewerMode == VIEWER_MODE_INPUT_ALPHA)
    {
        viewerSplit = false;
    }

    QString s;
    s.append(QString::number(static_cast<float>(viewerSplit)));
    s.append(",");
    s.append(splitSlider->getValuesAsString());
    s.append(",");
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

} // namespace Cascade
