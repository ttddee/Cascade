/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#include "gmicpropertiesentity.h"
#include "ui_gmicpropertiesentity.h"

#include <iostream>

#include <QTextStream>

#include "nodeproperties.h"
#include "propertiesheading.h"
#include "cssliderbox.h"
#include "colorbuttonentity.h"
#include "textbrowserentity.h"
#include "separatorentity.h"
#include "comboboxentity.h"
#include "lineeditentity.h"
#include "folderboxentity.h"

GmicPropertiesEntity::GmicPropertiesEntity(
        UIElementType et,
        const QString& gType,
        const QString& gHash,
        QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::GmicPropertiesEntity)
{
    ui->setupUi(this);

    gmicNodeType = gType;
    gmicHash = gHash;

    gmicHelper = &GmicHelper::getInstance();

    ui->verticalLayout->setAlignment(Qt::AlignTop);

    auto heading = new PropertiesHeading("G'MIC " + gType);
    ui->verticalLayout->addWidget(heading);

    std::cout << gmicHelper->getFiltersModel().getFilterFromHash(gHash).parameters().toStdString() << std::endl;

    auto parameters = gmicHelper->getFiltersModel().getFilterFromHash(gHash).parameters();
    QString line;
    QTextStream stream(&parameters);

    while (stream.readLineInto(&line))
    {
        // TEMP
        if (line.contains("(%)"))
        {
            line.remove(QRegExp("\\(%\\)"));
        }
        if (line.contains("(s)"))
        {
            line.remove(QRegExp("\\(s\\)"));
        }

        if (line.contains("=int") || line.contains("=_int"))
        {
            auto title = line.split("=")[0];
            auto values = line.split(QRegExp("[()]"))[1].split(",");
            auto value = values[0];
            auto min = values[1];
            auto max = values[2];

            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_INT,
                        this,
                        true);
            item->setName(title);
            item->setMinMaxStepValue(
                        min.toInt(),
                        max.toInt(),
                        1,
                        value.toInt());
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=float"))
        {
            auto title = line.split("=")[0];
            auto values = line.split(QRegExp("[()]"))[1].split(",");
            auto value = values[0];
            auto min = values[1];
            auto max = values[2];

            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                        this,
                        true);
            item->setName(title);
            item->setMinMaxStepValue(
                        min.toDouble(),
                        max.toDouble(),
                        0.01,
                        value.toDouble());
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=color"))
        {
            auto title = line.split("=")[0];
            auto value = line.split(QRegExp("[()]"))[1];

            ColorButtonEntity* item = new ColorButtonEntity(
                        UI_ELEMENT_TYPE_COLOR_BUTTON,
                        this);
            item->setName(title);
            item->setColor(QColor(value));
            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=note") && !line.contains("{"))
        {
            auto text = line.split("\"")[1];

            TextBrowserEntity* item = new TextBrowserEntity(
                        UI_ELEMENT_TYPE_TEXTBROWSER,
                        this);
            item->setText(text);
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=separator"))
        {
            SeparatorEntity* item = new SeparatorEntity(
                        UI_ELEMENT_TYPE_SEPARATOR,
                        this);
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=choice") || line.contains("=_choice"))
        {
            auto title = line.split("=")[0];
            auto elems = line.split(QRegExp("[()]"))[1];
            elems.remove(QRegExp("\""));
            auto values = elems.split(",");
            auto index = values[0];
            values.removeAt(0);

            ComboBoxEntity* item = new ComboBoxEntity(
                        UI_ELEMENT_TYPE_COMBOBOX,
                        this);
            item->setName(title);

            std::vector<QString> options;
            foreach (auto& i, values)
            {
                options.push_back(i);
            }
            item->setOptions(options, index.toInt());

            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=_bool"))
        {
            auto title = line.split("=")[0];
            auto value = line.split(QRegExp("[()]"))[1];

            CheckBoxEntity* item =
                    new CheckBoxEntity(
                        UI_ELEMENT_TYPE_CHECKBOX,
                        this);
            item->setName(title);
            item->setChecked(value.toInt());

            item->selfConnectToValueChanged(static_cast<NodeProperties*>(parent));
            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=_text") || line.contains("=text"))
        {
            auto title = line.split("=")[0];
            auto text = line.split(QRegExp("\""))[1];

            LineEditEntity* item = new LineEditEntity(
                        UI_ELEMENT_TYPE_LINEEDIT,
                        this);
            item->setName(title);
            item->setText(text);

            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
        else if (line.contains("=_folder"))
        {
            auto title = line.split("=")[0];

            FolderBoxEntity* item = new FolderBoxEntity(
                        UI_ELEMENT_TYPE_FOLDERBOX,
                        this);
            item->setName(title);

            ui->verticalLayout->addWidget(item);

            propElements.push_back(item);
        }
    }

    SeparatorEntity* item = new SeparatorEntity(
                UI_ELEMENT_TYPE_SEPARATOR,
                this);
    ui->verticalLayout->addWidget(item);

    propElements.push_back(item);

    previewBox = new CheckBoxEntity(
                UI_ELEMENT_TYPE_CHECKBOX,
                this);
    previewBox->setName("Preview");
    previewBox->setChecked(true);

    ui->verticalLayout->addWidget(previewBox);
    connect(previewBox, &CheckBoxEntity::valueChanged,
            this, &GmicPropertiesEntity::valueChanged);
    //propElements.push_back(previewBox);

    QSpacerItem* vertSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout->addItem(vertSpacer);
}

void GmicPropertiesEntity::selfConnectToValueChanged(NodeProperties* p)
{
    connect(this, &GmicPropertiesEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString GmicPropertiesEntity::getValuesAsString()
{
    QString output;

    if (previewBox->isChecked())
    {
        output.append(gmicHelper->getFiltersModel().getFilterFromHash(gmicHash).previewCommand());
    }
    else
    {
        output.append(gmicHelper->getFiltersModel().getFilterFromHash(gmicHash).command());
    }

    output.append(" ");

    foreach(auto& item, propElements)
    {
        output.append(item->getValuesAsString());
        if (item->getValuesAsString() != "")
        {
            output.append(",");
        }
    }
    output.chop(1);
    //output.append("2,0,50,50");
    std::cout << output.toStdString() << std::endl;
    return output;
}


GmicPropertiesEntity::~GmicPropertiesEntity()
{
    delete ui;
}
