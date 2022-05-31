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

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>

#include "intpropertyview.h"
#include "../nodegraph/propertymodel.h"

namespace Cascade::Properties {

IntPropertyView::IntPropertyView(QWidget *parent)
    : PropertyView(parent)
{
    setMinimumWidth(200);
    setMinimumHeight(22);
    setMaximumHeight(22);

    mLayout = new QGridLayout();
    mLayout->setVerticalSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mLayout);

    mSlider = new QSlider(Qt::Horizontal, this);
    mSlider->setMinimumHeight(22);
    mLayout->addWidget(mSlider);

    mNameLabel = new QLabel(this);
    mNameLabel->setText("None");
    mNameLabel->setObjectName("nameLabel");

    mLayout->addWidget(mNameLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
    mSpinBox = new QSpinBox(this);
    mSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    mLayout->addWidget(mSpinBox, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    mSlider->installEventFilter(this);
    mNameLabel->installEventFilter(this);

    connect(mSlider, &QSlider::valueChanged,
            this, &IntPropertyView::handleSliderValueChanged);
    connect(mSpinBox, &QSpinBox::valueChanged,
            this, &IntPropertyView::handleSpinBoxValueChanged);
}

void IntPropertyView::setModel(IntPropertyModel* model)
{
    mModel = model;
    mNameLabel->setText(mModel->getData()->getName());
    mSlider->setMinimum(mModel->getData()->getMin());
    mSlider->setMaximum(mModel->getData()->getMax());
    mSlider->setSingleStep(mModel->getData()->getStep());
    mBaseValue = mModel->getData()->getValue();
    mSlider->setValue(mBaseValue);
}

void IntPropertyView::setSpinBoxNoSignal(const int value)
{
    mSpinBox->blockSignals(true);
    mSpinBox->setValue(value);
    mSpinBox->blockSignals(false);
}

void IntPropertyView::setSliderNoSignal(const int value)
{
    mSlider->blockSignals(true);
    mSlider->setValue(value);
    mSlider->blockSignals(false);
}

void IntPropertyView::handleSliderValueChanged()
{
    int value = mSlider->value();
    mModel->setValue(value);
    setSpinBoxNoSignal(value);
}

void IntPropertyView::handleSpinBoxValueChanged()
{
    int value = mSpinBox->value();
    mModel->setValue(value);
    setSliderNoSignal(value);
}

bool IntPropertyView::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            mIsDragging = true;
            double factorPixelsToValue = mSlider->maximum() / static_cast<double>(this->size().width()); // factor for scaling pixels into value of slider
            mSlider->setValue( static_cast<int>(mouseEvent->x() *factorPixelsToValue));

            if (QApplication::queryKeyboardModifiers() == Qt::ControlModifier)
            {
                reset();
            }
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            mIsDragging = false;
        }
    }

    Q_UNUSED(watched);

    return false;
}

void IntPropertyView::reset()
{
    mSpinBox->setValue(mBaseValue);
}

void IntPropertyView::mouseMoveEvent(QMouseEvent* event)
{
    // lambda function to get sign of the value
    auto sign = [](int a) { return (0 < a) - (a < 0); };

    if(mIsDragging)
    {
        // factor for scaling pixels into value of slider
        double factorPixelsToValue = mSlider->maximum() / static_cast<double>(this->size().width());
        auto cursorPos = event->windowPos();
        int sliderValue = mSlider->value();
        auto offsetFromPrev = (cursorPos.x() - mLastPos.x());
        auto newPos = event->x() * factorPixelsToValue;

        // when shift is pressed we want to have more precision in the slider movement
        if (QApplication::queryKeyboardModifiers() == Qt::ShiftModifier)
            newPos = sliderValue + sign(offsetFromPrev) * mSlider->singleStep();

        mSlider->setValue(newPos);
        mLastPos = cursorPos;
    }
    Q_UNUSED(event);
}

} // namespace Cascade::Properties
