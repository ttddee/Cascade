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

#include "colorpropertiesentity.h"
#include "ui_colorpropertiesentity.h"

#include "cssliderbox.h"

ColorPropertiesEntity::ColorPropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::ColorPropertiesEntity)
{
    ui->setupUi(this);

    // Master
    CsSliderBox* item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Saturation");
    item->setMinMaxStepValue(0.0, 4.0, 0.01, 1.0);
    ui->verticalLayout->insertWidget(1, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Contrast");
    item->setMinMaxStepValue(0.0, 4.0, 0.01, 1.0);
    ui->verticalLayout->insertWidget(2, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gamma");
    item->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);
    ui->verticalLayout->insertWidget(3, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gain");
    item->setMinMaxStepValue(0.0, 4.0, 0.01, 1.0);
    ui->verticalLayout->insertWidget(4, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Lift");
    item->setMinMaxStepValue(-1.0, 1.0, 0.01, 0.0);
    ui->verticalLayout->insertWidget(5, item);

    // Shadows
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Saturation");
    ui->verticalLayout->insertWidget(7, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Contrast");
    ui->verticalLayout->insertWidget(8, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gamma");
    ui->verticalLayout->insertWidget(9, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gain");
    ui->verticalLayout->insertWidget(10, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Lift");
    ui->verticalLayout->insertWidget(11, item);

    // Midtones
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Saturation");
    ui->verticalLayout->insertWidget(13, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Contrast");
    ui->verticalLayout->insertWidget(14, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gamma");
    ui->verticalLayout->insertWidget(15, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gain");
    ui->verticalLayout->insertWidget(16, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Lift");
    ui->verticalLayout->insertWidget(17, item);

    // Highlights
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Saturation");
    ui->verticalLayout->insertWidget(19, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Contrast");
    ui->verticalLayout->insertWidget(20, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gamma");
    ui->verticalLayout->insertWidget(21, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Gain");
    ui->verticalLayout->insertWidget(22, item);
    item = new CsSliderBox(
                UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                this);
    item->setName("Lift");
    ui->verticalLayout->insertWidget(23, item);

}

QString ColorPropertiesEntity::getValuesAsString()
{
    return "";
}

ColorPropertiesEntity::~ColorPropertiesEntity()
{
    delete ui;
}
