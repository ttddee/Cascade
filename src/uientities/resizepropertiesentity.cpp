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

#include "../nodebase.h"

ResizePropertiesEntity::ResizePropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::ResizePropertiesEntity)
{
    ui->setupUi(this);

    connect(ui->widthPercent, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ResizePropertiesEntity::widthPercentChanged);
    connect(ui->heightPercent, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ResizePropertiesEntity::heightPercentChanged);
    connect(ui->widthPixels, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ResizePropertiesEntity::widthPixelsChanged);
    connect(ui->heightPixels, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ResizePropertiesEntity::heightPixelsChanged);
}

void ResizePropertiesEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ResizePropertiesEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString ResizePropertiesEntity::getValuesAsString()
{
    QString s = QString::number(ui->widthPixels->value());
    s.append(",");
    s.append(QString::number(ui->heightPixels->value()));
    s.append(",");
    s.append(QString::number(ui->linkCheckBox->isChecked()));

    return s;
}

void ResizePropertiesEntity::loadPropertyValues(const QString &values)
{
    emit valueChanged();

    auto split = values.split(",");
    setSpinBoxNoSignal(ui->widthPixels, split[0].toInt());
    setSpinBoxNoSignal(ui->heightPixels, split[1].toInt());
}

void ResizePropertiesEntity::handleNodeRequestUpdate()
{
    setInputSize(parentNode->getInputSize());
}

void ResizePropertiesEntity::setParentNode(NodeBase* node)
{
    parentNode = node;
}

void ResizePropertiesEntity::setInputSize(const QSize& s)
{
    if (inputSize != s)
    {
        inputSize = s;
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
    ui->linkCheckBox->setChecked(b);
}

void ResizePropertiesEntity::widthPercentChanged()
{
    factorX = static_cast<float>(ui->widthPercent->value()) / 100.0;
    updatePixelWidth();

    if (ui->linkCheckBox->isChecked())
    {
        factorY = factorX;
        updatePercentHeight();
        updatePixelHeight();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::heightPercentChanged()
{
    factorY = static_cast<float>(ui->heightPercent->value()) / 100.0;
    updatePixelHeight();

    if (ui->linkCheckBox->isChecked())
    {
        factorX = factorY;
        updatePercentWidth();
        updatePixelWidth();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::widthPixelsChanged()
{
    factorX = static_cast<float>(ui->widthPixels->value()) / inputSize.width();
    updatePercentWidth();

    if (ui->linkCheckBox->isChecked())
    {
        factorY = factorX;
        updatePercentHeight();
        updatePixelHeight();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::heightPixelsChanged()
{
    factorY = static_cast<float>(ui->heightPixels->value()) / inputSize.height();
    updatePercentHeight();

    if (ui->linkCheckBox->isChecked())
    {
        factorX = factorY;
        updatePercentWidth();
        updatePixelWidth();
    }

    emit valueChanged();
}

void ResizePropertiesEntity::updatePercentWidth()
{
    setDoubleSpinBoxNoSignal(ui->widthPercent, factorX * 100);
}

void ResizePropertiesEntity::updatePercentHeight()
{
    setDoubleSpinBoxNoSignal(ui->heightPercent, factorY * 100);
}

void ResizePropertiesEntity::updatePixelWidth()
{
    setSpinBoxNoSignal(ui->widthPixels, inputSize.width() * factorX);
}

void ResizePropertiesEntity::updatePixelHeight()
{
    setSpinBoxNoSignal(ui->heightPixels, inputSize.height() * factorY);
}

double ResizePropertiesEntity::getWidthPercent() const
{
    return ui->widthPercent->value();
}

double ResizePropertiesEntity::getHeightPercent() const
{
    return ui->heightPercent->value();
}

void ResizePropertiesEntity::setWidthPercent(const double d)
{
    ui->widthPercent->setValue(d);
}

void ResizePropertiesEntity::setHeightPercent(const double d)
{
    ui->heightPercent->setValue(d);
}

void ResizePropertiesEntity::setWidthPixels(const int i)
{
    ui->widthPixels->setValue(i);
}

void ResizePropertiesEntity::setHeightPixels(const int i)
{
    ui->heightPixels->setValue(i);
}

ResizePropertiesEntity::~ResizePropertiesEntity()
{
    delete ui;
}
