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

#include "channelselectentity.h"
#include "ui_channelselectentity.h"

namespace Cascade {

ChannelSelectEntity::ChannelSelectEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    mUi(new Ui::ChannelSelectEntity)
{
    mUi->setupUi(this);

    connect(mUi->redBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(mUi->greenBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(mUi->blueBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(mUi->alphaBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
}

void ChannelSelectEntity::hideAlphaChannel()
{
    mUi->alphaBox->setHidden(true);
}

void ChannelSelectEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ChannelSelectEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString ChannelSelectEntity::getValuesAsString()
{
    auto s = QString::number(mUi->redBox->isChecked()) + "," +
             QString::number(mUi->greenBox->isChecked()) + "," +
             QString::number(mUi->blueBox->isChecked()) + "," +
             QString::number(mUi->alphaBox->isChecked());
    return s;
}

void ChannelSelectEntity::loadPropertyValues(const QString &values)
{
    auto split = values.split(",");
    std::vector<QCheckBox*> boxes = {
        mUi->redBox, mUi->greenBox, mUi->blueBox, mUi->alphaBox
    };
    for (int i = 0; i < split.size(); i++)
    {
        split[i].toInt() == 1 ?
                    boxes[i]->setCheckState(Qt::CheckState::Checked) :
                    boxes[i]->setCheckState(Qt::CheckState::Unchecked);
    }
}

ChannelSelectEntity::~ChannelSelectEntity()
{
    delete mUi;
}

} // namespace Cascade
