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

#include "sizeboxentity.h"
#include "ui_sizeboxentity.h"

#include "../global.h"

namespace Cascade {

SizeBoxEntity::SizeBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::SizeBoxEntity)
{
    mUi->setupUi(this);

    mUi->modeComboBox->addItem("As Input");
    mUi->modeComboBox->addItem("Custom");

    QMapIterator<QString, QPair<int, int>> i(Config::sImageSizePresets);
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        mUi->modeComboBox->addItem(i.key());
    }

    connect(mUi->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SizeBoxEntity::handleSelectionChanged);
    connect(mUi->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SizeBoxEntity::valueChanged);
    connect(mUi->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SizeBoxEntity::valueChanged);

    hideCustomSizeElements();
}

void SizeBoxEntity::hideCustomSizeElements()
{
    mUi->widthLabel->setHidden(true);
    mUi->widthSpinBox->setHidden(true);
    mUi->heightLabel->setHidden(true);
    mUi->heightSpinBox->setHidden(true);
    mUi->bottomLine->setHidden(true);
}

void SizeBoxEntity::unHideCustomSizeElements()
{
    mUi->widthLabel->setHidden(false);
    mUi->widthSpinBox->setHidden(false);
    mUi->heightLabel->setHidden(false);
    mUi->heightSpinBox->setHidden(false);
    mUi->bottomLine->setHidden(false);
}

void SizeBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
//    connect(this, &SizeBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString SizeBoxEntity::getValuesAsString()
{
    if (mUi->modeComboBox->currentIndex() == 0)
    {
        return "";
    }
    else if (mUi->modeComboBox->currentIndex() == 1)
    {
        QString size =
                QString::number(mUi->widthSpinBox->value()) +
                "," +
                QString::number(mUi->heightSpinBox->value());
        return size;
    }
    else
    {
        auto k = mUi->modeComboBox->currentText();
        QString size =
                QString::number(Config::sImageSizePresets[k].first) +
                "," +
                QString::number(Config::sImageSizePresets[k].second);
        return size;
    }
}

void SizeBoxEntity::loadPropertyValues(const QString &values)
{
    if (values == "")
    {
        mUi->modeComboBox->setCurrentIndex(0);
    }
    else
    {
        auto split = values.split(",");
        mUi->modeComboBox->setCurrentIndex(1);
        mUi->widthSpinBox->setValue(split[0].toInt());
        mUi->heightSpinBox->setValue(split[1].toInt());
        handleSelectionChanged();
    }
}

void SizeBoxEntity::setSizeBoxNoSignal(const QSize& size)
{
    mUi->widthSpinBox->blockSignals(true);
    mUi->heightSpinBox->blockSignals(true);
    mUi->widthSpinBox->setValue(size.width());
    mUi->heightSpinBox->setValue(size.height());
    mUi->widthSpinBox->blockSignals(false);
    mUi->heightSpinBox->blockSignals(false);
}

void SizeBoxEntity::handleSelectionChanged()
{
    if (mUi->modeComboBox->currentIndex() == 1)
    {
        unHideCustomSizeElements();
    }
    else
    {
        hideCustomSizeElements();
    }
    if (mUi->modeComboBox->currentIndex() > 1)
    {
        QString item = mUi->modeComboBox->currentText();
        setSizeBoxNoSignal(QSize(
                           Config::sImageSizePresets[item].first,
                           Config::sImageSizePresets[item].second));
    }
    emit valueChanged();
}

SizeBoxEntity::~SizeBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
