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

#include "colorbutton.h"

#include <iostream>

#include <QColorDialog>
#include <QPainter>
#include <QPaintEvent>

ColorButton::ColorButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setText("");

    connect(this, &ColorButton::clicked,
            this, &ColorButton::handleButtonClicked);

    color = Qt::white;
}

QColor ColorButton::getColor()
{
    return color;
}

void ColorButton::setColor(QColor c)
{
    color = c;

    emit colorChanged(c);
}

void ColorButton::handleButtonClicked()
{
    QColor c = QColorDialog::getColor(
                this->getColor(),
                nullptr,
                QString(),
                QColorDialog::ShowAlphaChannel);

    if (c.isValid())
        setColor(c);
}

void ColorButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);

    QRect r(3, 3, 24, 14);
    auto c = this->getColor();
    c.setAlpha(255);
    painter.setBrush(c);
    painter.setPen(Qt::NoPen);
    painter.drawRect(r);
}
