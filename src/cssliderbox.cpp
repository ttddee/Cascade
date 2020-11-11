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

    valueBox = new QDoubleSpinBox(this);
    valueBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->gridLayout->addWidget(valueBox, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    ui->slider->installEventFilter(this);
    nameLabel->installEventFilter(this);
    valueBox->installEventFilter(this);

    connect(ui->slider, &SliderNoClick::valueChanged,
            this, &CsSliderBox::setSpinBoxNoSignal);
    connect(valueBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CsSliderBox::setSliderNoSignal);
}

void CsSliderBox::setMinMaxStepValue(
        double min,
        double max,
        double step,
        double value)
{
    ui->slider->setMinimum(min * 100);
    ui->slider->setMaximum(max * 100);
    ui->slider->setSingleStep(step * 100);
    ui->slider->setValue(value * 100);
    valueBox->setMinimum(min);
    valueBox->setMaximum(max);
    valueBox->setSingleStep(step);
    valueBox->setValue(value);

    baseValue = value;
}

void CsSliderBox::selfConnectToValueChanged(NodeProperties* p)
{
    connect(this, &CsSliderBox::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

void CsSliderBox::setSpinBoxNoSignal(int i)
{
    valueBox->blockSignals(true);
    valueBox->setValue(i / 100.0);
    valueBox->blockSignals(false);

    emit valueChanged(valueBox->value());
}

void CsSliderBox::setSliderNoSignal(double d)
{
    ui->slider->blockSignals(true);
    ui->slider->setValue(static_cast<int>(d * 100));
    ui->slider->blockSignals(false);

    emit valueChanged(valueBox->value());
}

void CsSliderBox::setName(const QString &name)
{
    nameLabel->setText(name);
}

QString CsSliderBox::getValuesAsString()
{
    return QString::number(ui->slider->value() / 100.0);
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

            if (controlPressed)
            {
                valueBox->setValue(baseValue);
            }
        }
    }

    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Control)
        {
            controlPressed = true;
        }
    }

    if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Control)
        {
            controlPressed = false;
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
