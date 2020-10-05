#include "nodegraphcontextmenu.h"

#include <iostream>

#include "nodedefinitions.h"
#include "nodegraph.h"

using namespace Cascade;

NodeGraphContextMenu::NodeGraphContextMenu(NodeGraph* parent)
    : QMenu(parent)
{
    this->setFixedWidth(150);

    // Populate menu with submenus aka categories
    QMap<NodeCategory, QMenu*> categories;
    {
        QMapIterator<NodeCategory, QString> i(categoryStrings);
        while (i.hasNext())
        {
            i.next();
            categories[i.key()] = this->addMenu(categoryStrings[i.key()]);
        }
    }

    // Add nodes to corresponding submenus
    QMapIterator<NodeType, QString> i(nodeStrings);
    while (i.hasNext())
    {
        i.next();
        auto a = new QAction();
        actions.push_back(a);
        a->setText(i.value());
        auto t = i.key();
        categories[getPropertiesForType(t).category]->addAction(a);

        QObject::connect(
                    a,
                    &QAction::triggered,
                    parent,
                    [parent, t]{ parent->createNode(t); });
    }
}
