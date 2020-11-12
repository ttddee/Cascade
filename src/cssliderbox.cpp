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

#include "cssliderbox.h"
#include "ui_cssliderbox.h"

#include <iostream>

#include <QMouseEvent>

#include "nodeproperties.h"

CsSliderBox::CsSliderBox(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::CsSliderBox)
{
    ui->setupUi(this);

    nameLabel = new QLabel(this);
    nameLabel->setText("None");
    nameLabel->setObjectName("nameLabel");
    ui->gridLayout->addWidget(nameLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
    {
        valueBoxDouble = new QDoubleSpinBox(this);
        valueBoxDouble->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->gridLayout->addWidget(valueBoxDouble, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        valueBoxDouble->installEventFilter(this);
    }
    else
    {
        valueBoxInt = new QSpinBox(this);
        valueBoxInt->setButtonSymbols(QAbstractSpinBox::NoButtons);
        ui->gridLayout->addWidget(valueBoxInt, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        valueBoxInt->installEventFilter(this);
    }

    ui->slider->installEventFilter(this);
    nameLabel->installEventFilter(this);

    connect(ui->slider, &SliderNoClick::valueChanged,
            this, &CsSliderBox::setSpinBoxNoSignal);
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
    {
        connect(valueBoxDouble, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &CsSliderBox::setSliderNoSignalDouble);
    }
    else
    {
        connect(valueBoxInt, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &CsSliderBox::setSliderNoSignalInt);
    }

}

void CsSliderBox::setMinMaxStepValue(
        double min,
        double max,
        double step,
        double value)
{
    ui->slider->setMinimum(min * 1000);
    ui->slider->setMaximum(max * 1000);
    ui->slider->setSingleStep(step * 1000);
    ui->slider->setValue(value * 1000);
    valueBoxDouble->setMinimum(min);
    valueBoxDouble->setMaximum(max);
    valueBoxDouble->setSingleStep(step);
    valueBoxDouble->setValue(value);

    baseValue = value;
}

void CsSliderBox::setMinMaxStepValue(
        int min,
        int max,
        int step,
        int value)
{
    ui->slider->setMinimum(min * 10);
    ui->slider->setMaximum(max * 10);
    ui->slider->setSingleStep(step);
    ui->slider->setValue(value * 10);
    valueBoxInt->setMinimum(min);
    valueBoxInt->setMaximum(max);
    valueBoxInt->setSingleStep(step);
    valueBoxInt->setValue(value);
}

void CsSliderBox::selfConnectToValueChanged(NodeProperties* p)
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
    {
        connect(this, &CsSliderBox::valueChangedDouble,
                p, [p]{p->handleSomeValueChanged();});
    }
    else
    {
        connect(this, &CsSliderBox::valueChangedInt,
                p, [p]{p->handleSomeValueChanged();});
    }

}

void CsSliderBox::setSpinBoxNoSignal(int i)
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
    {
        valueBoxDouble->blockSignals(true);
        valueBoxDouble->setValue(i / 1000.0);
        valueBoxDouble->blockSignals(false);

        emit valueChangedDouble(valueBoxDouble->value());
    }
    else
    {
        valueBoxInt->blockSignals(true);
        valueBoxInt->setValue(i / 10);
        valueBoxInt->blockSignals(false);

        emit valueChangedInt(valueBoxInt->value());
    }

}

void CsSliderBox::setSliderNoSignalDouble(double d)
{
    ui->slider->blockSignals(true);
    ui->slider->setValue(static_cast<int>(d * 1000));
    ui->slider->blockSignals(false);

    emit valueChangedDouble(valueBoxDouble->value());
}

void CsSliderBox::setSliderNoSignalInt(int i)
{
    ui->slider->blockSignals(true);
    ui->slider->setValue(i * 10);
    ui->slider->blockSignals(false);

    emit valueChangedInt(valueBoxInt->value());
}

void CsSliderBox::setName(const QString &name)
{
    nameLabel->setText(name);
}

QString CsSliderBox::getValuesAsString()
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
    {
        return QString::number(ui->slider->value() / 1000.0);
    }
    else
    {
        return QString::number(ui->slider->value() / 10);
    }
}

bool CsSliderBox::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            lastPos = QCursor::pos();
            isDragging = true;

            if (QApplication::queryKeyboardModifiers() == Qt::ControlModifier)
            {
                valueBoxDouble->setValue(baseValue);
            }
        }
    }

    Q_UNUSED(watched);

    return false;
}

void CsSliderBox::mouseMoveEvent(QMouseEvent* event)
{
    if(isDragging)
    {
        float dx = QCursor::pos().x() - lastPos.x();
        int lastValue = ui->slider->value();
        int newValue = lastValue + dx * ui->slider->singleStep();

        ui->slider->setValue(newValue);
    }
    lastPos = QCursor::pos();

    Q_UNUSED(event);
}

void CsSliderBox::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
}

CsSliderBox::~CsSliderBox()
{
    delete ui;
}
