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

#include "resizepropertiesentity.h"
#include "ui_resizepropertiesentity.h"

//#include "../nodegraph/nodebase.h"

namespace Cascade {

ResizePropertiesEntity::ResizePropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::ResizePropertiesEntity)
{
    mUi->setupUi(this);

    connect(mUi->widthPercent, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ResizePropertiesEntity::widthPercentChanged);
    connect(mUi->heightPercent, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ResizePropertiesEntity::heightPercentChanged);
    connect(mUi->widthPixels, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ResizePropertiesEntity::widthPixelsChanged);
    connect(mUi->heightPixels, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ResizePropertiesEntity::heightPixelsChanged);
}

void ResizePropertiesEntity::selfConnectToValueChanged(NodeProperties *p)
{
//    connect(this, &ResizePropertiesEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString ResizePropertiesEntity::getValuesAsString()
{
    QString s = QString::number(mUi->widthPixels->value());
    s.append(",");
    s.append(QString::number(mUi->heightPixels->value()));
    s.append(",");
    s.append(QString::number(mUi->linkCheckBox->isChecked()));

    return s;
}

void ResizePropertiesEntity::loadPropertyValues(const QString &values)
{
    emit valueChanged();

    auto split = values.split(",");
    setSpinBoxNoSignal(mUi->widthPixels, split[0].toInt());
    setSpinBoxNoSignal(mUi->heightPixels, split[1].toInt());
}

void ResizePropertiesEntity::handleNodeRequestUpdate()
{
    //setInputSize(mParentNode->getInputSize());
}

void ResizePropertiesEntity::setParentNode(NodeBase* node)
{
    mParentNode = node;
}

void ResizePropertiesEntity::setInputSize(const QSize& s)
{
    if (mInputSize != s)
    {
        mInputSize = s;
        updatePercentWidth();
        updatePercentHeight();
        updatePixelWidth();
        updatePixelHeight();
    }
}

void ResizePropertiesEntity::setSpinBoxNoSignal(QSpinBox* box, int i)
{
    box->blockSignals(true);
    box->setValue(i);
    box->blockSignals(false);
}

void ResizePropertiesEntity::setDoubleSpinBoxNoSignal(QDoubleSpinBox* box, double d)
{
    box->blockSignals(true);
    box->setValue(d);
    box->blockSignals(false);
}

void ResizePropertiesEntity::setLinked(bool b)
{
    mUi->linkCheckBox->setChecked(b);
}

void ResizePropertiesEntity::widthPercentChanged()
{
    mFactorX = static_cast<float>(mUi->widthPercent->value()) / 100.0;
    updatePixelWidth();

    if (mUi->linkCheckBox->isChecked())
    {
        mFactorY = mFactorX;
        updatePercentHeight();
        updatePixelHeight();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::heightPercentChanged()
{
    mFactorY = static_cast<float>(mUi->heightPercent->value()) / 100.0;
    updatePixelHeight();

    if (mUi->linkCheckBox->isChecked())
    {
        mFactorX = mFactorY;
        updatePercentWidth();
        updatePixelWidth();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::widthPixelsChanged()
{
    mFactorX = static_cast<float>(mUi->widthPixels->value()) / mInputSize.width();
    updatePercentWidth();

    if (mUi->linkCheckBox->isChecked())
    {
        mFactorY = mFactorX;
        updatePercentHeight();
        updatePixelHeight();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::heightPixelsChanged()
{
    mFactorY = static_cast<float>(mUi->heightPixels->value()) / mInputSize.height();
    updatePercentHeight();

    if (mUi->linkCheckBox->isChecked())
    {
        mFactorX = mFactorY;
        updatePercentWidth();
        updatePixelWidth();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::updatePercentWidth()
{
    setDoubleSpinBoxNoSignal(mUi->widthPercent, mFactorX * 100);
}

void ResizePropertiesEntity::updatePercentHeight()
{
    setDoubleSpinBoxNoSignal(mUi->heightPercent, mFactorY * 100);
}

void ResizePropertiesEntity::updatePixelWidth()
{
    setSpinBoxNoSignal(mUi->widthPixels, mInputSize.width() * mFactorX);
}

void ResizePropertiesEntity::updatePixelHeight()
{
    setSpinBoxNoSignal(mUi->heightPixels, mInputSize.height() * mFactorY);
}

double ResizePropertiesEntity::getWidthPercent() const
{
    return mUi->widthPercent->value();
}

double ResizePropertiesEntity::getHeightPercent() const
{
    return mUi->heightPercent->value();
}

void ResizePropertiesEntity::setWidthPercent(const double d)
{
    mUi->widthPercent->setValue(d);
}

void ResizePropertiesEntity::setHeightPercent(const double d)
{
    mUi->heightPercent->setValue(d);
}

void ResizePropertiesEntity::setWidthPixels(const int i)
{
    mUi->widthPixels->setValue(i);
}

void ResizePropertiesEntity::setHeightPixels(const int i)
{
    mUi->heightPixels->setValue(i);
}

ResizePropertiesEntity::~ResizePropertiesEntity()
{
    delete mUi;
}

} // namespace Cascade
