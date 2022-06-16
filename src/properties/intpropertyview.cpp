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

#include <QApplication>
#include <QEvent>
#include <QMouseEvent>

#include "intpropertyview.h"
#include "../properties/propertymodel.h"

#include "../log.h"

using Cascade::UiElements::SliderType;

namespace Cascade::Properties {

IntPropertyView::IntPropertyView(QWidget *parent)
    : PropertyView(parent)
{
    mLayout = new QGridLayout();
    mLayout->setVerticalSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mLayout);

    mSlider = new Slider(SliderType::Int, this);
    mLayout->addWidget(mSlider);
}

void IntPropertyView::setModel(IntPropertyModel* model)
{
    mModel = model;
    mSlider->setName(mModel->getData()->getName());
    mSlider->setMinMaxStepValue(
        mModel->getData()->getMin(),
        mModel->getData()->getMax(),
        mModel->getData()->getStep(),
        mModel->getData()->getValue());
}

} // namespace Cascade::Properties
