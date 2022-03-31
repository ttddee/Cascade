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
    ui(new Ui::SpinBoxEntity)
{
    ui->setupUi(this);

    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this]{ emit valueChanged(); });
    connect(ui->resetButton, &QPushButton::clicked,
            this, &SpinBoxEntity::reset);
}

const QString SpinBoxEntity::name()
{
    return ui->nameLabel->text();
}

void SpinBoxEntity::setName(const QString &name)
{
    ui->nameLabel->setText(name);
}

void SpinBoxEntity::setMinMaxStepValue(int min, int max, int step, int value)
{
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
    ui->spinBox->setSingleStep(step);
    ui->spinBox->setValue(value);
    baseValue = value;
}

void SpinBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &SpinBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

void SpinBoxEntity::reset()
{
    ui->spinBox->setValue(baseValue);
    emit valueChanged();
}

QString SpinBoxEntity::getValuesAsString()
{
    return QString::number(ui->spinBox->value());
}

void SpinBoxEntity::loadPropertyValues(const QString &values)
{
    ui->spinBox->setValue(values.toInt());
}

SpinBoxEntity::~SpinBoxEntity()
{
    delete ui;
}

} // namespace Cascade
