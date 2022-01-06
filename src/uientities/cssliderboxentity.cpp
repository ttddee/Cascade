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

#include "cssliderboxentity.h"

#include <QMouseEvent>

#include "../nodeproperties.h"

CsSliderBoxEntity::CsSliderBoxEntity(UIElementType et, QWidget *parent, bool onlyUpdateOnSliderRelease) :
    UiEntity(et, parent),
    ui(new Ui::CsSliderBox)
{
    ui->setupUi(this);

    this->onlyUpdateOnSliderRelease = onlyUpdateOnSliderRelease;

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

//    if (onlyUpdateOnSliderRelease)
//    {
//        connect(ui->slider, &SliderNoClick::sliderReleased,
//                this, [this](){ setSpinBoxNoSignal(ui->slider->value()); });
//        if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
//        {
//            connect(ui->slider, &SliderNoClick::sliderReleased,
//                    this, [this](){ emit valueChangedDouble(valueBoxDouble->value()); });
//            connect(valueBoxDouble, &QDoubleSpinBox::editingFinished,
//                    this, [this](){ emit valueChangedDouble(valueBoxDouble->value()); });
//            connect(valueBoxDouble, &QDoubleSpinBox::editingFinished,
//                    this, [this](){ setSliderNoSignal(valueBoxDouble->value()); });
//        }
//        else
//        {
//            connect(ui->slider, &SliderNoClick::sliderReleased,
//                    this, [this](){ emit valueChangedInt(valueBoxInt->value()); });
//            connect(valueBoxInt, &QSpinBox::editingFinished,
//                    this, [this](){ emit valueChangedInt(valueBoxInt->value()); });
//            connect(valueBoxInt, &QSpinBox::editingFinished,
//                    this, [this](){ setSliderNoSignal(valueBoxInt->value()); });
//        }
//    }
    connect(ui->slider, &SliderNoClick::valueChanged,
            this, &CsSliderBoxEntity::handleSliderValueChanged);

    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        connect(valueBoxDouble, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &CsSliderBoxEntity::handleSpinBoxValueChanged);
    else
        connect(valueBoxInt, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &CsSliderBoxEntity::handleSpinBoxValueChanged);
}

void CsSliderBoxEntity::selfConnectToValueChanged(NodeProperties* p)
{
    connect(this, &CsSliderBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

const QString CsSliderBoxEntity::name()
{
    return nameLabel->text();
}

void CsSliderBoxEntity::setName(const QString &name)
{
    nameLabel->setText(name);
}

QString CsSliderBoxEntity::getValuesAsString()
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        return QString::number(ui->slider->value() / DOUBLE_MULT);
    else
        return QString::number(ui->slider->value());
}

void CsSliderBoxEntity::handleSliderValueChanged()
{
    setSpinBoxNoSignal(ui->slider->value());
}

void CsSliderBoxEntity::handleSpinBoxValueChanged()
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        setSliderNoSignal(valueBoxDouble->value());
    else
        setSliderNoSignal(valueBoxInt->value());
}

void CsSliderBoxEntity::reset()
{
    if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        valueBoxDouble->setValue(baseValue);
    else
        valueBoxInt->setValue(baseValue);
}

bool CsSliderBoxEntity::eventFilter(QObject *watched, QEvent *event)
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
                reset();
            }
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            isDragging = false;
            if(onlyUpdateOnSliderRelease)
                emit valueChanged();
        }
    }

    Q_UNUSED(watched);

    return false;
}

void CsSliderBoxEntity::mouseMoveEvent(QMouseEvent* event)
{
    if(isDragging)
    {
        float dx = QCursor::pos().x() - lastPos.x();
        int lastValue = ui->slider->value();
        float offset = dx * (ui->slider->singleStep() * ((ui->slider->maximum() - ui->slider->minimum()) / 500.0));
        if (offset > 0.0 && offset  < 1.0)
            offset = 1.0;
        else if (offset > -1.0 && offset < 0.0)
            offset = -1.0;
        int newValue = lastValue + offset;

        ui->slider->setValue(newValue);
    }
    lastPos = QCursor::pos();

    Q_UNUSED(event);
}

CsSliderBoxEntity::~CsSliderBoxEntity()
{
    delete ui;
}
