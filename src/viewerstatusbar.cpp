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

    QMapIterator<ViewerMode, QString> i(sViewerModeText);
    while (i.hasNext())
    {
        i.next();
        ui->viewerModeBox->addItem(i.value());
    }
    ui->viewerModeBox->setCurrentIndex(3);
    ui->viewerModeBox->setMinimumWidth(90);

    mSplitSlider = new Slider(
        SliderType::Double,
        this);
    mSplitSlider->setName("Split");
    mSplitSlider->setMaximumWidth(250);
    mSplitSlider->setMinMaxStepValue(0.0, 1.0, 0.01, 0.5);
    ui->horizontalLayout->insertWidget(11, mSplitSlider);

    mGammaSlider = new Slider(
        SliderType::Double,
        this);
    mGammaSlider->setName("Gamma");
    mGammaSlider->setMaximumWidth(250);
    mGammaSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(15, mGammaSlider);

    mGainSlider = new Slider(
        SliderType::Double,
        this);
    mGainSlider->setName("Gain");
    mGainSlider->setMaximumWidth(250);
    mGainSlider->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->horizontalLayout->insertWidget(16, mGainSlider);

    connect(ui->zoomResetButton, &QPushButton::clicked,
            this, &ViewerStatusBar::requestZoomReset);
    connect(ui->splitCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleSplitToggled);
    connect(ui->bwCheckBox, &QCheckBox::toggled,
            this, &ViewerStatusBar::handleBwToggled);
    connect(mGammaSlider, &Slider::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
    connect(mGainSlider, &Slider::valueChanged,
            this, &ViewerStatusBar::handleValueChanged);
    connect(mSplitSlider, &Slider::valueChanged,
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

void ViewerStatusBar::handleSplitToggled()
{
    if(!mSplit)
        mSplit = true;
    else
        mSplit = false;
    emit valueChanged();
}

void ViewerStatusBar::handleSplitSliderChanged()
{
    if (mSplit)
    {
        emit valueChanged();
    }
}

void ViewerStatusBar::handleBwToggled()
{
    if(!mBw)
        mBw = true;
    else
        mBw = false;
    emit valueChanged();
}

void ViewerStatusBar::handleValueChanged()
{
    emit valueChanged();
}

void ViewerStatusBar::handleViewerModeCheckBoxChanged()
{
    currentViewerMode = sViewerModeText.key(ui->viewerModeBox->currentText());
    emit viewerModeChanged(currentViewerMode);
    ui->viewerModeBox->clearFocus();

    emit valueChanged();
}

void ViewerStatusBar::handleSwitchToViewerMode(const ViewerMode mode)
{
    currentViewerMode = mode;
    ui->viewerModeBox->setCurrentText(sViewerModeText.value(mode));
}

QString ViewerStatusBar::getViewerSettings()
{
    // Only use set split viewer on RGB Out or Alpha Out view
    bool viewerSplit = mSplit;
    if (currentViewerMode == ViewerMode::Back ||
        currentViewerMode == ViewerMode::Front ||
        currentViewerMode == ViewerMode::Alpha)
    {
        viewerSplit = false;
    }

    QString s;
//    s.append(QString::number(static_cast<float>(viewerSplit)));
//    s.append(",");
//    s.append(mSplitSlider->getValuesAsString());
//    s.append(",");
//    s.append(QString::number(static_cast<float>(mBw)));
//    s.append(",");
//    s.append(mGammaSlider->getValuesAsString());
//    s.append(",");
//    s.append(mGainSlider->getValuesAsString());

    return s;
}

ViewerStatusBar::~ViewerStatusBar()
{
    delete ui;
}

} // namespace Cascade
