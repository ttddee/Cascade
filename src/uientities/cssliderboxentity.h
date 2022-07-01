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

        double factor{1.0};
        if (std::is_same<T , double>::value)
        {
            factor = DOUBLE_MULT;
        }

        mValueBox->blockSignals(true);
        static_cast<QDoubleSpinBox*>(mValueBox)->setMinimum(min);
        static_cast<QDoubleSpinBox*>(mValueBox)->setMaximum(max);
        static_cast<QDoubleSpinBox*>(mValueBox)->setSingleStep(step);
        static_cast<QDoubleSpinBox*>(mValueBox)->setValue(value);
        mValueBox->blockSignals(false);
        mUi->slider->blockSignals(true);
        mUi->slider->setMinimum(min *= factor);
        mUi->slider->setMaximum(max *= factor);
        mUi->slider->setSingleStep(step *= factor);
        mUi->slider->setValue(value *= factor);
        mUi->slider->blockSignals(false);


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
            mValueBox->blockSignals(true);
            static_cast<QDoubleSpinBox*>(mValueBox)->setValue(val / DOUBLE_MULT);
            mValueBox->blockSignals(false);
        }
        else
        {
            mValueBox->blockSignals(true);
            static_cast<QSpinBox*>(mValueBox)->setValue(val);
            mValueBox->blockSignals(false);
        }

        emit valueChanged();
    }

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;

    Ui::CsSliderBox *mUi;

    QLabel* mNameLabel;
    QAbstractSpinBox* mValueBox; // can be QDoubleSpinBox or QSpinBox (for int values)
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
