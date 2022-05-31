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

#include "propertieswindow.h"

#include "../log.h"

namespace Cascade::Properties
{

PropertiesWindow::PropertiesWindow(QWidget *parent)
    : QWidget(parent)
{
    mLayout = new QVBoxLayout();
    mLayout->setAlignment(Qt::AlignTop);
    mLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mLayout);
    this->setAttribute(Qt::WA_StyledBackground);
}

void PropertiesWindow::setPropertyWidget(PropertyWidget *widget)
{
    clear();

    mLayout->addWidget(widget);
    mPropertyWidget = widget;
    mPropertyWidget->show();
}

void PropertiesWindow::clear()
{
    if (mPropertyWidget)
    {
        mPropertyWidget->hide();
        mLayout->removeWidget(mPropertyWidget);
        mPropertyWidget = nullptr;
    }
}

void PropertiesWindow::handleActiveNodeChanged(Node* node)
{
    setPropertyWidget(node->propertyWidget());
}

} // namespace Cascade::Properties
