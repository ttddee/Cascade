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

#include "colorbuttonentity.h"
#include "ui_colorbuttonentity.h"

namespace Cascade {

ColorButtonEntity::ColorButtonEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::ColorButtonEntity)
{
    mUi->setupUi(this);

    connect(mUi->colorButton, &ColorButton::colorChanged,
                this, &ColorButtonEntity::handleColorChanged);
}

QColor ColorButtonEntity::getColor()
{
    return mUi->colorButton->getColor();
}

void ColorButtonEntity::setColor(QColor c)
{
    mUi->colorButton->setColor(c);
}

const QString ColorButtonEntity::name()
{
    return mUi->label->text();
}

void ColorButtonEntity::setName(const QString &s)
{
    mUi->label->setText(s);
}

void ColorButtonEntity::handleColorChanged(QColor c)
{
    emit valueChanged();

    Q_UNUSED(c);
}

QString ColorButtonEntity::getValuesAsString()
{
    QString s;
    s.append(QString::number(getColor().red() / 255.0));
    s.append(",");
    s.append(QString::number(getColor().green() / 255.0));
    s.append(",");
    s.append(QString::number(getColor().blue() / 255.0));
    s.append(",");
    s.append(QString::number(getColor().alpha() / 255.0));
    return s;
}

void ColorButtonEntity::loadPropertyValues(const QString &values)
{
    auto split = values.split(",");
    QColor c(split[0].toInt() * 255.0,
             split[1].toInt() * 255.0,
             split[2].toInt() * 255.0,
             split[3].toInt() * 255.0);
    setColor(c);
}

void ColorButtonEntity::selfConnectToValueChanged(NodeProperties *p)
{
//    connect(this, &ColorButtonEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

ColorButtonEntity::~ColorButtonEntity()
{
    delete mUi;
}

} // namespace Cascade
