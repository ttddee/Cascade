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

#include <QWidget>
#include <QLabel>
#include <QSpinBox>

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

    void setMinMaxStepValue(double, double, double, double);
    void setMinMaxStepValue(int, int, int, int);
    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~CsSliderBox();

private:
    void setSpinBoxNoSignal(int i);
    void setSliderNoSignalDouble(double d);
    void setSliderNoSignalInt(int i);

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

};

#endif // CSSLIDERBOX_H
