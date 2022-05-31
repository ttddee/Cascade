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

#ifndef INTPROPERTYVIEW_H
#define INTPROPERTYVIEW_H

#include <QObject>

#include "propertyview.h"

namespace Cascade::NodeGraph
{
    class IntPropertyModel;
}

using Cascade::NodeGraph::IntPropertyModel;

namespace Cascade::Properties {

class IntPropertyView : public PropertyView
{
    Q_OBJECT

public:
    IntPropertyView(QWidget *parent = nullptr);

    void setModel(IntPropertyModel* model);

private:
    void setSpinBoxNoSignal(const int value);
    void setSliderNoSignal(const int value);

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;
    void mouseMoveEvent(QMouseEvent*) override;

    IntPropertyModel* mModel;

    QGridLayout* mLayout;
    QSlider* mSlider;
    QLabel* mNameLabel;
    QSpinBox* mSpinBox;

    int mBaseValue = 0;
    bool mIsDragging = false;
    QPointF mLastPos;

private slots:
    void handleSliderValueChanged();
    void handleSpinBoxValueChanged();
};

} // namespace Cascade::Properties

#endif // INTPROPERTYVIEW_H
