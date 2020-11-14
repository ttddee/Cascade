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

#include "nodeproperties.h"

#include <iostream>

#include <QLabel>

#include "fileboxentity.h"
#include "propertiesheading.h"
#include "nodebase.h"
#include "colorbuttonentity.h"
#include "writepropertiesentity.h"
#include "spinboxentity.h"
#include "comboboxentity.h"
#include "channelselectentity.h"
#include "cssliderbox.h"
#include "colorpropertiesentity.h"
#include "sizeboxentity.h"
#include "textboxentity.h"

NodeProperties::NodeProperties(
        const NodeType t,
        NodeBase* parentNode,
        QWidget *parent)
        : QWidget(parent),
          nodeType(t),
          parentNode(parentNode)
{
    NodeInitProperties props = Cascade::getPropertiesForType(t);

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setMargin(0);
    this->setLayout(layout);

    foreach (auto& elem, props.uiElements)
    {
        if(elem.first == UI_ELEMENT_TYPE_PROPERTIES_HEADING)
        {
            PropertiesHeading* item = new PropertiesHeading(
                        elem.second,
                        this);
            layout->addWidget(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SPINBOX)
        {
            SpinBoxEntity* item =
                    new SpinBoxEntity(
                        UI_ELEMENT_TYPE_SPINBOX,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_FILEBOX)
        {
            FileBoxEntity* item = new FileBoxEntity(
                        UI_ELEMENT_TYPE_FILEBOX,
                        this);
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_COLOR_BUTTON)
        {
            ColorButtonEntity* item = new ColorButtonEntity(
                        UI_ELEMENT_TYPE_COLOR_BUTTON,
                        this);
            item->selfConnectToValueChanged(this);
            item->setName(elem.second);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_WRITE_PROPERTIES)
        {
            WritePropertiesEntity* item = new WritePropertiesEntity(
                        UI_ELEMENT_TYPE_WRITE_PROPERTIES,
                        this);
            item->selfConnectToRequestFileSave(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_COMBOBOX)
        {
            ComboBoxEntity* item = new ComboBoxEntity(
                        UI_ELEMENT_TYPE_COMBOBOX,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts[0]);
            std::vector<QString> options;
            for (int i = 1; i < parts.size() - 1; ++i)
            {
                options.push_back(parts[i]);
            }
            item->setOptions(options, parts.last().toInt());
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_CHANNEL_SELECT)
        {
            ChannelSelectEntity* item = new ChannelSelectEntity(
                        UI_ELEMENT_TYPE_CHANNEL_SELECT,
                        this);
            auto parts = elem.second.split(",");
            if (parts.at(0).toInt() == 1)
            {
                item->hideAlphaChannel();
            }
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE)
        {
            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toDouble(),
                        parts.at(2).toDouble(),
                        parts.at(3).toDouble(),
                        parts.at(4).toDouble());
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SLIDER_BOX_INT)
        {
            CsSliderBox* item =
                    new CsSliderBox(
                        UI_ELEMENT_TYPE_SLIDER_BOX_INT,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_COLOR_PROPERTIES)
        {
            ColorPropertiesEntity* item = new ColorPropertiesEntity(
                        UI_ELEMENT_TYPE_COLOR_PROPERTIES,
                        this);
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SIZEBOX)
        {
            SizeBoxEntity* item = new SizeBoxEntity(
                        UI_ELEMENT_TYPE_SIZEBOX,
                        this);
            item->selfConnectToValueChanged(this);
            layout->addWidget(item);
            widgets.push_back(item);
            parentNode->setHasCustomSize(item);
        }
        else if (elem.first == UI_ELEMENT_TYPE_TEXTBOX)
        {
            TextBoxEntity* item = new TextBoxEntity(
                        UI_ELEMENT_TYPE_TEXTBOX,
                        this);
            item->setText(elem.second);
            layout->addWidget(item);
            widgets.push_back(item);
        }
    }
}

void NodeProperties::handleSomeValueChanged()
{
    parentNode->requestUpdate();
}

void NodeProperties::handleFileSaveRequest(const QString& path)
{
    emit parentNode->nodeRequestFileSave(parentNode, path);
}
