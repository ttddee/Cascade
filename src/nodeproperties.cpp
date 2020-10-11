#include "nodeproperties.h"

#include <iostream>

#include <QLabel>

#include "fileboxentity.h"
#include "propertiesheading.h"
#include "spinboxsliderentity.h"
#include "nodebase.h"
#include "colorbuttonentity.h"
#include "writepropertiesentity.h"
#include "spinboxentity.h"
#include "comboboxentity.h"


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
        else if (elem.first == UI_ELEMENT_TYPE_SLIDERSPIN_INT)
        {
            SpinBoxSliderEntity* item =
                    new SpinBoxSliderEntity(
                        UI_ELEMENT_TYPE_SLIDERSPIN_INT,
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
        else if (elem.first == UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE)
        {
            SpinBoxSliderEntity* item =
                    new SpinBoxSliderEntity(
                        UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE,
                        this);
            item->makeDouble();
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
    }
}

void NodeProperties::handleSomeValueChanged()
{
    std::cout << "Node value changed, requesting update." << std::endl;
    parentNode->requestUpdate();
}

void NodeProperties::handleFileSaveRequest(const QString& path)
{
    std::cout << path.toStdString() << std::endl;
    emit parentNode->nodeRequestFileSave(parentNode, path);
}
