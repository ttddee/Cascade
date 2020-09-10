#include "nodeproperties.h"

#include <QLabel>

#include "fileboxentity.h"

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
            QLabel* l = new QLabel(i.value(), this);
            layout->addWidget(l);
        }
        else if (i.key() == UI_ELEMENT_TYPE_SLIDERSPIN_INT)
        {

        }
        else if (i.key() == UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE)
        {

        }
        else if (i.key() == UI_ELEMENT_TYPE_FILEBOX)
        {
            FileBoxEntity* f = new FileBoxEntity(UI_ELEMENT_TYPE_FILEBOX, this);
            layout->addWidget(f);
            widgets.push_back(f);
        }
    }


}
