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

#include "comboboxentity.h"
#include "ui_comboboxentity.h"

ComboBoxEntity::ComboBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::ComboBoxEntity)
{
    ui->setupUi(this);

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]{ emit valueChanged();});
}

const QString ComboBoxEntity::name()
{
    return ui->label->text();
}

void ComboBoxEntity::setName(const QString &name)
{
    ui->label->setText(name);
}

void ComboBoxEntity::setOptions(const std::vector<QString> options, int index)
{
    foreach(auto& o, options)
    {
        ui->comboBox->addItem(o);
    }
    ui->comboBox->setCurrentIndex(index);
}

void ComboBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ComboBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString ComboBoxEntity::getValuesAsString()
{
    return QString::number(ui->comboBox->currentIndex());
}

ComboBoxEntity::~ComboBoxEntity()
{
    delete ui;
}
