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

//#include "../nodegraph/nodeproperties.h"

namespace Cascade {

CsSliderBoxEntity::CsSliderBoxEntity(
        UIElementType et,
        QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::CsSliderBox)
{
    mUi->setupUi(this);

    mNameLabel = new QLabel(this);
    mNameLabel->setText("None");
    mNameLabel->setObjectName("nameLabel");
    mUi->gridLayout->addWidget(mNameLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    mValueBox = new QDoubleSpinBox(this);
    mValueBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    mUi->gridLayout->addWidget(mValueBox, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    mValueBox->installEventFilter(this);
    auto valueBoxSizeHint = mValueBox->sizeHint();
    mValueBox->setMinimumWidth(valueBoxSizeHint.width()*1.5);
    if (mElementType == UIElementType::eSliderBoxDouble)
    {
        mIsDouble = true;
    }
    else
    {
        mValueBox->setDecimals(0);
    }

    mUi->slider->installEventFilter(this);
    mNameLabel->installEventFilter(this);

    connect(mUi->slider, &SliderNoClick::valueChanged,
            this, &CsSliderBoxEntity::handleSliderValueChanged);

    connect(mValueBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CsSliderBoxEntity::handleSpinBoxValueChanged);
}

void CsSliderBoxEntity::selfConnectToValueChanged( [[maybe_unused]] NodeProperties* p)
{
//    connect(this, &CsSliderBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

const QString CsSliderBoxEntity::name()
{
    return mNameLabel->text();
}

void CsSliderBoxEntity::setName(const QString &name)
{
    mNameLabel->setText(name);
}

QString CsSliderBoxEntity::getValuesAsString()
{
    if (mElementType == UIElementType::eSliderBoxDouble)
        return QString::number(mUi->slider->value() / DOUBLE_MULT);
    else
        return QString::number(mUi->slider->value());
}

void CsSliderBoxEntity::loadPropertyValues(const QString &values)
{
    if (mIsDouble)
        mValueBox->setValue(values.toFloat());
    else
        mValueBox->setValue(values.toInt());
}

void CsSliderBoxEntity::handleSliderValueChanged()
{
    setSpinBoxNoSignal(mUi->slider->value());
}

void CsSliderBoxEntity::handleSpinBoxValueChanged()
{
    setSliderNoSignal(mValueBox->value());
}

void CsSliderBoxEntity::reset()
{
    mValueBox->setValue(mBaseValue);
}

bool CsSliderBoxEntity::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            mIsDragging = true;
            double factorPixelsToValue = mUi->slider->maximum() / static_cast<double>(this->size().width()); // factor for scaling pixels into value of slider
            mUi->slider->setValue( static_cast<int>(mouseEvent->x() *factorPixelsToValue));

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

void CsSliderBoxEntity::mouseMoveEvent(QMouseEvent* event)
{
    // lambda function to get sign of the value
    auto sign = [](int a) { return (0 < a) - (a < 0); };

    if(mIsDragging)
    {
        // factor for scaling pixels into value of slider
        double factorPixelsToValue = mUi->slider->maximum() / static_cast<double>(this->size().width());
        auto cursorPos = event->windowPos();
        int sliderValue = mUi->slider->value();
        auto offsetFromPrev = (cursorPos.x() - mLastPos.x());
        auto newPos = event->x() * factorPixelsToValue;

        // when shift is pressed we want to have more precision in the slider movement
        if (QApplication::queryKeyboardModifiers() == Qt::ShiftModifier)
            newPos = sliderValue + sign(offsetFromPrev) * mUi->slider->singleStep();

        mUi->slider->setValue(newPos);
        mLastPos = cursorPos;
     }
    Q_UNUSED(event);
}


CsSliderBoxEntity::~CsSliderBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
