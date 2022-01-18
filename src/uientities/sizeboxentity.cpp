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

SizeBoxEntity::SizeBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::SizeBoxEntity)
{
    ui->setupUi(this);

    ui->modeComboBox->addItem("As Input");
    ui->modeComboBox->addItem("Custom");

    QMapIterator<QString, QPair<int, int>> i(sizePresets);
    i.toBack();
    while (i.hasPrevious())
    {
        i.previous();
        ui->modeComboBox->addItem(i.key());
    }

    connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SizeBoxEntity::handleSelectionChanged);
    connect(ui->widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SizeBoxEntity::valueChanged);
    connect(ui->heightSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SizeBoxEntity::valueChanged);

    hideCustomSizeElements();
}

void SizeBoxEntity::hideCustomSizeElements()
{
    ui->widthLabel->setHidden(true);
    ui->widthSpinBox->setHidden(true);
    ui->heightLabel->setHidden(true);
    ui->heightSpinBox->setHidden(true);
    ui->bottomLine->setHidden(true);
}

void SizeBoxEntity::unHideCustomSizeElements()
{
    ui->widthLabel->setHidden(false);
    ui->widthSpinBox->setHidden(false);
    ui->heightLabel->setHidden(false);
    ui->heightSpinBox->setHidden(false);
    ui->bottomLine->setHidden(false);
}

void SizeBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &SizeBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString SizeBoxEntity::getValuesAsString()
{
    if (ui->modeComboBox->currentIndex() == 0)
    {
        return "";
    }
    else if (ui->modeComboBox->currentIndex() == 1)
    {
        QString size =
                QString::number(ui->widthSpinBox->value()) +
                "," +
                QString::number(ui->heightSpinBox->value());
        return size;
    }
    else
    {
        auto k = ui->modeComboBox->currentText();
        QString size =
                QString::number(sizePresets[k].first) +
                "," +
                QString::number(sizePresets[k].second);
        return size;
    }
}

void SizeBoxEntity::loadPropertyValues(const QString &values)
{
    CS_LOG_CONSOLE(values);
    if (values == "")
    {
        ui->modeComboBox->setCurrentIndex(0);
    }
    else
    {
        auto split = values.split(",");
        ui->modeComboBox->setCurrentIndex(1);
        ui->widthSpinBox->setValue(split[0].toInt());
        ui->heightSpinBox->setValue(split[1].toInt());
        handleSelectionChanged();
    }
}

void SizeBoxEntity::setSizeBoxNoSignal(const QSize& size)
{
    ui->widthSpinBox->blockSignals(true);
    ui->heightSpinBox->blockSignals(true);
    ui->widthSpinBox->setValue(size.width());
    ui->heightSpinBox->setValue(size.height());
    ui->widthSpinBox->blockSignals(false);
    ui->heightSpinBox->blockSignals(false);
}

void SizeBoxEntity::handleSelectionChanged()
{
    if (ui->modeComboBox->currentIndex() == 1)
    {
        unHideCustomSizeElements();
    }
    else
    {
        hideCustomSizeElements();
    }
    if (ui->modeComboBox->currentIndex() > 1)
    {
        QString item = ui->modeComboBox->currentText();
        setSizeBoxNoSignal(QSize(
                           sizePresets[item].first,
                           sizePresets[item].second));
    }
    emit valueChanged();
}

SizeBoxEntity::~SizeBoxEntity()
{
    delete ui;
}
