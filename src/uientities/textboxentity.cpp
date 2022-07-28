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

#include "textboxentity.h"
#include "ui_textboxentity.h"

namespace Cascade {

TextBoxEntity::TextBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::TextBoxEntity)
{
    mUi->setupUi(this);

    mUi->textEdit->setReadOnly(true);
}

void TextBoxEntity::setText(const QString &s)
{
    mUi->textEdit->document()->setPlainText(s);
}

QString TextBoxEntity::getValuesAsString()
{
    return "";
}

void TextBoxEntity::loadPropertyValues( [[maybe_unused]] const QString &values)
{

}

TextBoxEntity::~TextBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
