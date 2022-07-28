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

#include "comboboxentity.h"
#include "ui_comboboxentity.h"

namespace Cascade {

ComboBoxEntity::ComboBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::ComboBoxEntity)
{
    mUi->setupUi(this);

    connect(mUi->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]{ emit valueChanged();});
}

const QString ComboBoxEntity::name()
{
    return mUi->label->text();
}

void ComboBoxEntity::setName(const QString &name)
{
    mUi->label->setText(name);
}

void ComboBoxEntity::setOptions(const std::vector<QString> options, int index)
{
    foreach(auto& o, options)
    {
        mUi->comboBox->addItem(o);
    }
    mUi->comboBox->setCurrentIndex(index);
}

void ComboBoxEntity::selfConnectToValueChanged( [[maybe_unused]] NodeProperties *p)
{
//    connect(this, &ComboBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString ComboBoxEntity::getValuesAsString()
{
    return QString::number(mUi->comboBox->currentIndex());
}

void ComboBoxEntity::loadPropertyValues(const QString &values)
{
    mUi->comboBox->setCurrentIndex(values.toInt());
}

ComboBoxEntity::~ComboBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
