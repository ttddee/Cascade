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

#include "titlepropertyview.h"

#include "titlepropertymodel.h"

namespace Cascade::Properties
{

TitlePropertyView::TitlePropertyView(QWidget* parent)
    : PropertyView(parent)
{
    mLayout = new QVBoxLayout(this);
    mLayout->setSpacing(5);
    mLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mLayout);

    mTitleLabel = new QLabel(this);
    mTitleLabel->setObjectName("PropertiesTitleLabel");
    mLayout->addWidget(mTitleLabel);

    QFrame* line;
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mLayout->addWidget(line);
}

void TitlePropertyView::setModel(TitlePropertyModel* model)
{
    mModel = model;
    mTitleLabel->setText(mModel->getData()->getTitle());
}

} // namespace Cascade::Properties
