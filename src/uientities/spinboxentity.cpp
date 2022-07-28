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

#include "spinboxentity.h"
#include "ui_spinboxentity.h"

namespace Cascade {

SpinBoxEntity::SpinBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::SpinBoxEntity)
{
    mUi->setupUi(this);

    connect(mUi->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this]{ emit valueChanged(); });
    connect(mUi->resetButton, &QPushButton::clicked,
            this, &SpinBoxEntity::reset);
}

const QString SpinBoxEntity::name()
{
    return mUi->nameLabel->text();
}

void SpinBoxEntity::setName(const QString &name)
{
    mUi->nameLabel->setText(name);
}

void SpinBoxEntity::setMinMaxStepValue(int min, int max, int step, int value)
{
    mUi->spinBox->setMinimum(min);
    mUi->spinBox->setMaximum(max);
    mUi->spinBox->setSingleStep(step);
    mUi->spinBox->setValue(value);
    mBaseValue = value;
}

void SpinBoxEntity::selfConnectToValueChanged( [[maybe_unused]] NodeProperties *p)
{
//    connect(this, &SpinBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

void SpinBoxEntity::reset()
{
    mUi->spinBox->setValue(mBaseValue);
    emit valueChanged();
}

QString SpinBoxEntity::getValuesAsString()
{
    return QString::number(mUi->spinBox->value());
}

void SpinBoxEntity::loadPropertyValues(const QString &values)
{
    mUi->spinBox->setValue(values.toInt());
}

SpinBoxEntity::~SpinBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
