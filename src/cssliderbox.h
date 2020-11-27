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
#ifndef CSSLIDERBOX_H
#define CSSLIDERBOX_H

#include "ui_cssliderbox.h"

#include <iostream>

#include <QWidget>
#include <QLabel>
#include <QSpinBox>

#include <gtest/gtest_prod.h>

#include "uientity.h"

class NodeProperties;

#define DOUBLE_MULT 100.0

namespace Ui {
class CsSliderBox;
}

class CsSliderBox : public UiEntity
{
    Q_OBJECT

public:
    explicit CsSliderBox(UIElementType et, QWidget *parent = nullptr, bool onlyUpdateOnSliderRelease = false);

    template<typename T>
    void setMinMaxStepValue(
            T min,
            T max,
            T step,
            T value)
    {
        baseValue = value;

        if (std::is_same<T , double>::value)
        {
            valueBoxDouble->blockSignals(true);
            valueBoxDouble->setMinimum(min);
            valueBoxDouble->setMaximum(max);
            valueBoxDouble->setSingleStep(step);
            valueBoxDouble->setValue(value);
            valueBoxDouble->blockSignals(false);

            ui->slider->blockSignals(true);
            ui->slider->setMinimum(min *= DOUBLE_MULT);
            ui->slider->setMaximum(max *= DOUBLE_MULT);
            ui->slider->setSingleStep(step *= DOUBLE_MULT);
            ui->slider->setValue(value *= DOUBLE_MULT);
            ui->slider->blockSignals(false);
        }
        else
        {
            valueBoxInt->blockSignals(true);
            valueBoxInt->setMinimum(min);
            valueBoxInt->setMaximum(max);
            valueBoxInt->setSingleStep(step);
            valueBoxInt->setValue(value);
            valueBoxInt->blockSignals(false);

            ui->slider->blockSignals(true);
            ui->slider->setMinimum(min);
            ui->slider->setMaximum(max);
            ui->slider->setSingleStep(step);
            ui->slider->setValue(value);
            ui->slider->blockSignals(false);
        }
    }

    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~CsSliderBox();

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
        ui->slider->blockSignals(true);
        ui->slider->setValue(static_cast<int>(val));
        ui->slider->blockSignals(false);

        if(!onlyUpdateOnSliderRelease)
            emit valueChanged();
    }

    template<typename T>
    void setSpinBoxNoSignal(T val)
    {
        if (elementType == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        {
            valueBoxDouble->blockSignals(true);
            valueBoxDouble->setValue(val / DOUBLE_MULT);
            valueBoxDouble->blockSignals(false);
        }
        else
        {
            valueBoxInt->blockSignals(true);
            valueBoxInt->setValue(val);
            valueBoxInt->blockSignals(false);
        }
        if(!onlyUpdateOnSliderRelease)
            emit valueChanged();
    }

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;

    Ui::CsSliderBox *ui;

    QLabel* nameLabel;
    QDoubleSpinBox* valueBoxDouble;
    QSpinBox* valueBoxInt;

    bool isDragging = false;
    QPoint lastPos;

    float baseValue;
    bool onlyUpdateOnSliderRelease = false;

signals:
    void valueChanged();

private slots:
    void handleSliderValueChanged();
    void handleSpinBoxValueChanged();
};

#endif // CSSLIDERBOX_H
