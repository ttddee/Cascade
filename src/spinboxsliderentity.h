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

#ifndef SPINBOXSLIDERENTITY_H
#define SPINBOXSLIDERENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class SpinBoxSliderEntity;
}

class SpinBoxSliderEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SpinBoxSliderEntity(UIElementType et, QWidget *parent = nullptr);

    void makeDouble();
    void setName(const QString& name);
    void setMinMaxStepValue(int, int, int, int);
    void setMinMaxStepValue(double, double, double, double);
    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    auto value()
    {
        return isDouble ? currentValue / 100.0 : currentValue;
    }

    ~SpinBoxSliderEntity();

private:
    void setSpinBoxNoSignal(int);
    void setSliderNoSignal(int);
    void setSliderNoSignal(double);
    void setCurrentValue(int);
    void setCurrentValue(double);
    void reset();

    Ui::SpinBoxSliderEntity *ui;

    bool isDouble = false;
    int baseValue = 0;
    int currentValue = 0;

signals:
    void valueChangedInt(int);
    void valueChangedDouble(double);

private slots:
    void handleSliderValueChanged();

};

#endif // SPINBOXSLIDERENTITY_H
