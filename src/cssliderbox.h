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
        if (std::is_same<T , double>::value)
        {
            ui->slider->setMinimum(min *= 100);
            ui->slider->setMaximum(max *= 100);
            ui->slider->setSingleStep(step *= 100);
            ui->slider->setValue(value *= 100);

            valueBoxDouble->setMinimum(min);
            valueBoxDouble->setMaximum(max);
            valueBoxDouble->setSingleStep(step);
            valueBoxDouble->setValue(value);
        }
        else
        {
            ui->slider->setMinimum(min);
            ui->slider->setMaximum(max);
            ui->slider->setSingleStep(step);
            ui->slider->setValue(value);

            valueBoxInt->setMinimum(min);
            valueBoxInt->setMaximum(max);
            valueBoxInt->setSingleStep(step);
            valueBoxInt->setValue(value);
        }

        baseValue = value;
    }

    //void setMinMaxStepValue(int, int, int, int);
    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~CsSliderBox();

private:
    FRIEND_TEST(CsSliderBoxTest, setMinMaxStepValue_SetValuesDoubleBox);
    FRIEND_TEST(CsSliderBoxTest, setMinMaxStepValue_SetValuesIntBox);

    void setSpinBoxNoSignal(int i);

    template<typename T>
    void setSliderNoSignal(T);

//    void setSliderNoSignalDouble(double d);
//    void setSliderNoSignalInt(int i);

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

    Ui::CsSliderBox *ui;

    QLabel* nameLabel;
    QDoubleSpinBox* valueBoxDouble;
    QSpinBox* valueBoxInt;

    bool isDragging = false;
    QPoint lastPos;

    float baseValue;
    bool onlyUpdateOnSliderRelease = false;

signals:
    void valueChangedDouble(double d);
    void valueChangedInt(int i);
    void valueChanged();

};

#endif // CSSLIDERBOX_H
