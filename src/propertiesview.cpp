#include "propertiesview.h"

#include <iostream>

PropertiesView::PropertiesView(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);
}

void PropertiesView::loadProperties(NodeProperties *prop)
{
    if (currentProperties)
    {
        layout->removeWidget(currentProperties);
        currentProperties->hide();
    }
    layout->addWidget(prop);
    currentProperties = prop;
    currentProperties->show();
}
