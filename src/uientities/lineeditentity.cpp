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

#include "lineeditentity.h"
#include "ui_lineeditentity.h"

namespace Cascade {

LineEditEntity::LineEditEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::LineEditEntity)
{
    mUi->setupUi(this);

    connect(mUi->lineEdit, &QLineEdit::editingFinished,
            this, &LineEditEntity::valueChanged);
}

const QString LineEditEntity::name()
{
    return mUi->label->text();
}

void LineEditEntity::setName(const QString &name)
{
    mUi->label->setText(name);
}

void LineEditEntity::setText(const QString &text)
{
    mUi->lineEdit->setText(text);
}

void LineEditEntity::selfConnectToValueChanged(NodeProperties *p)
{
//    connect(this, &LineEditEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString LineEditEntity::getValuesAsString()
{
    return "\"" + mUi->lineEdit->text() + "\"";
}

void LineEditEntity::loadPropertyValues(const QString &values)
{

}

LineEditEntity::~LineEditEntity()
{
    delete mUi;
}

} //namespace Cascade
