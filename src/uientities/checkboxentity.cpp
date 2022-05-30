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

#include "checkboxentity.h"
#include "ui_checkboxentity.h"

namespace Cascade {

CheckBoxEntity::CheckBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::CheckBoxEntity)
{
    mUi->setupUi(this);

    connect(mUi->checkBox, &QCheckBox::stateChanged,
            this, &CheckBoxEntity::valueChanged);
}

const QString CheckBoxEntity::name()
{
    return mUi->checkBox->text();
}

void CheckBoxEntity::setName(const QString &name)
{
    mUi->checkBox->setText(name);
}

void CheckBoxEntity::setChecked(bool b)
{
    mUi->checkBox->setChecked(b);
}

bool CheckBoxEntity::isChecked()
{
    return mUi->checkBox->isChecked();
}

void CheckBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
//    connect(this, &CheckBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString CheckBoxEntity::getValuesAsString()
{
    return QString::number(mUi->checkBox->isChecked());
}

void CheckBoxEntity::loadPropertyValues(const QString &values)
{
    values.toInt() == 1 ?
                mUi->checkBox->setCheckState(Qt::CheckState::Checked) :
                mUi->checkBox->setCheckState(Qt::CheckState::Unchecked);
}

CheckBoxEntity::~CheckBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
