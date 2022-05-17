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
#ifndef CSSLIDERBOX_H
#define CSSLIDERBOX_H

#include "ui_cssliderboxentity.h"

#include <QWidget>
#include <QLabel>
#include <QSpinBox>

#include <gtest/gtest_prod.h>

#include "uientity.h"

#define DOUBLE_MULT 100.0

namespace Ui {
class CsSliderBox;
}

namespace Cascade {

class NodeProperties;

class CsSliderBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit CsSliderBoxEntity
    (UIElementType et,
     QWidget *parent = nullptr);

    template<typename T>
    void setMinMaxStepValue(
            T min,
            T max,
            T step,
            T value)
    {
        mBaseValue = value;

        if (std::is_same<T , double>::value)
        {
            mValueBoxDouble->blockSignals(true);
            mValueBoxDouble->setMinimum(min);
            mValueBoxDouble->setMaximum(max);
            mValueBoxDouble->setSingleStep(step);
            mValueBoxDouble->setValue(value);
            mValueBoxDouble->blockSignals(false);

            mUi->slider->blockSignals(true);
            mUi->slider->setMinimum(min *= DOUBLE_MULT);
            mUi->slider->setMaximum(max *= DOUBLE_MULT);
            mUi->slider->setSingleStep(step *= DOUBLE_MULT);
            mUi->slider->setValue(value *= DOUBLE_MULT);
            mUi->slider->blockSignals(false);
        }
        else
        {
            mValueBoxInt->blockSignals(true);
            mValueBoxInt->setMinimum(min);
            mValueBoxInt->setMaximum(max);
            mValueBoxInt->setSingleStep(step);
            mValueBoxInt->setValue(value);
            mValueBoxInt->blockSignals(false);

            mUi->slider->blockSignals(true);
            mUi->slider->setMinimum(min);
            mUi->slider->setMaximum(max);
            mUi->slider->setSingleStep(step);
            mUi->slider->setValue(value);
            mUi->slider->blockSignals(false);
        }
    }

    const QString name() override;
    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    ~CsSliderBoxEntity();

private:
    FRIEND_TEST(CsSliderBoxTest, setMinMaxStepValue_SetValuesDoubleBox);
    FRIEND_TEST(CsSliderBoxTest, setMinMaxStepValue_SetValuesIntBox);
    FRIEND_TEST(CsSliderBoxTest, setSliderValue_DoubleSpinBoxHasCorrectValue);
    FRIEND_TEST(CsSliderBoxTest, setSliderValue_IntSpinBoxHasCorrectValue);
    FRIEND_TEST(CsSliderBoxTest, setDoubleSpinBoxValue_SliderHasCorrectValue);
    FRIEND_TEST(CsSliderBoxTest, setIntSpinBoxValue_SliderHasCorrectValue);
    FRIEND_TEST(CsSliderBoxTest, resetDouble_AllValuesCorrect);
    FRIEND_TEST(CsSliderBoxTest, resetInt_AllValuesCorrect);
    FRIEND_TEST(CsSliderBoxTest, getValuesAsString_DoubleValueCorrect);
    FRIEND_TEST(CsSliderBoxTest, getValuesAsString_IntValueCorrect);

    template<typename T>
    void setSliderNoSignal(T val)
    {
        if (std::is_same<T , double>::value)
        {
            val *= DOUBLE_MULT;
        }
        mUi->slider->blockSignals(true);
        mUi->slider->setValue(static_cast<int>(val));
        mUi->slider->blockSignals(false);

        emit valueChanged();
    }

    template<typename T>
    void setSpinBoxNoSignal(T val)
    {
        if (mElementType == UIElementType::eSliderBoxDouble)
        {
            mValueBoxDouble->blockSignals(true);
            mValueBoxDouble->setValue(val / DOUBLE_MULT);
            mValueBoxDouble->blockSignals(false);
        }
        else
        {
            mValueBoxInt->blockSignals(true);
            mValueBoxInt->setValue(val);
            mValueBoxInt->blockSignals(false);
        }

        emit valueChanged();
    }

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;

    Ui::CsSliderBox *mUi;

    QLabel* mNameLabel;
    QDoubleSpinBox* mValueBoxDouble;
    QSpinBox* mValueBoxInt;

    bool mIsDragging = false;
    bool mIsDouble = false;
    float mBaseValue;
    QPointF mLastPos;

signals:
    void valueChanged();

private slots:
    void handleSliderValueChanged();
    void handleSpinBoxValueChanged();
};

} // namespace Cascade

#endif // CSSLIDERBOX_H
