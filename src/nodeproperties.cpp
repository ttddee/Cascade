#include "nodeproperties.h"

#include <iostream>

#include <QLabel>

#include "spinboxentity.h"
#include "fileboxentity.h"
#include "propertiesheading.h"
#include "spinboxsliderentity.h"
#include "nodebase.h"
#include "colorbuttonentity.h"

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
            PropertiesHeading* heading = new PropertiesHeading(
                        elem.second,
                        this);
            layout->addWidget(heading);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SLIDERSPIN_INT)
        {
            SpinBoxSliderEntity* box =
                    new SpinBoxSliderEntity(
                        UI_ELEMENT_TYPE_SLIDERSPIN_INT,
                        this);
            auto parts = elem.second.split(",");
            box->setName(parts.at(0));
            box->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            box->selfConnectToValueChanged(this);
            layout->addWidget(box);
            widgets.push_back(box);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE)
        {
            SpinBoxSliderEntity* box =
                    new SpinBoxSliderEntity(
                        UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE,
                        this);
            box->makeDouble();
            auto parts = elem.second.split(",");
            box->setName(parts.at(0));
            box->setMinMaxStepValue(
                        parts.at(1).toDouble(),
                        parts.at(2).toDouble(),
                        parts.at(3).toDouble(),
                        parts.at(4).toDouble());
            box->selfConnectToValueChanged(this);
            layout->addWidget(box);
            widgets.push_back(box);
        }
        else if (elem.first == UI_ELEMENT_TYPE_SPINBOX)
        {
            SpinBoxEntity* box =
                    new SpinBoxEntity(
                        UI_ELEMENT_TYPE_SPINBOX,
                        this);
            auto parts = elem.second.split(",");
            box->setName(parts.at(0));
            box->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            box->selfConnectToValueChanged(this);
            layout->addWidget(box);
            widgets.push_back(box);
        }
        else if (elem.first == UI_ELEMENT_TYPE_FILEBOX)
        {
            FileBoxEntity* f = new FileBoxEntity(
                        UI_ELEMENT_TYPE_FILEBOX,
                        this);
            f->selfConnectToValueChanged(this);
            layout->addWidget(f);
            widgets.push_back(f);
        }
        else if (elem.first == UI_ELEMENT_TYPE_COLOR_BUTTON)
        {
            ColorButtonEntity* b = new ColorButtonEntity(
                        UI_ELEMENT_TYPE_COLOR_BUTTON,
                        this);
            b->selfConnectToValueChanged(this);
            b->setName(elem.second);
            layout->addWidget(b);
            widgets.push_back(b);
        }
    }
}

void NodeProperties::handleSomeValueChanged()
{
    std::cout << "Node value changed, requesting update." << std::endl;
    parentNode->requestUpdate();
}
