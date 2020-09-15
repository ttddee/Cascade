#include "nodeproperties.h"

#include <iostream>

#include <QLabel>

#include "fileboxentity.h"
#include "propertiesheading.h"
#include "spinboxsliderentity.h"

NodeProperties::NodeProperties(const NodeType t, QWidget *parent)
    : QWidget(parent),
      nodeType(t)
{
    NodeInitProperties props = Cascade::getPropertiesForType(t);

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    this->setLayout(layout);

    QMapIterator<UIElementType, QString> i(props.uiElements);
    while(i.hasNext())
    {
        i.next();
        if (i.key() == UI_ELEMENT_TYPE_PROPERTIES_HEADING)
        {
            PropertiesHeading* heading = new PropertiesHeading(i.value(), this);
            layout->addWidget(heading);
        }
        else if (i.key() == UI_ELEMENT_TYPE_SLIDERSPIN_INT)
        {
            SpinBoxSliderEntity* box = new SpinBoxSliderEntity(this);
            auto parts = i.value().split(",");
            box->setName(parts.at(0));
            box->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            box->selfConnectToValueChanged(this);
            layout->addWidget(box);
        }
        else if (i.key() == UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE)
        {
            SpinBoxSliderEntity* box = new SpinBoxSliderEntity(this);
            box->makeDouble();
            auto parts = i.value().split(",");
            box->setName(parts.at(0));
            box->setMinMaxStepValue(
                        parts.at(1).toDouble(),
                        parts.at(2).toDouble(),
                        parts.at(3).toDouble(),
                        parts.at(4).toDouble());
            box->selfConnectToValueChanged(this);
            layout->addWidget(box);
        }
        else if (i.key() == UI_ELEMENT_TYPE_FILEBOX)
        {
            FileBoxEntity* f = new FileBoxEntity(UI_ELEMENT_TYPE_FILEBOX, this);
            layout->addWidget(f);
            widgets.push_back(f);
        }
    }
}

void NodeProperties::handleSomeValueChanged()
{
    std::cout << "beep" << std::endl;
}
