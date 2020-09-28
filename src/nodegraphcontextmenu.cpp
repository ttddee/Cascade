#include "nodegraphcontextmenu.h"

#include <iostream>

#include "nodedefinitions.h"
#include "nodegraph.h"

using namespace Cascade;

NodeGraphContextMenu::NodeGraphContextMenu(NodeGraph* parent)
    : QMenu(parent)
{
    this->setFixedWidth(150);

    // Add Read Node at the top
    auto a = new QAction();
    actions.push_back(a);
    a->setText("Read");
    this->addAction(a);
    auto t = NODE_TYPE_READ;
    QObject::connect(
                a,
                &QAction::triggered,
                parent,
                [parent, t]{ parent->createNode(t); });

    this->addSeparator();

    QMapIterator<NodeType, QString> i(nodeStrings);
    while (i.hasNext())
    {
        i.next();
        if (i.key() != NODE_TYPE_READ)
        {
            auto a = new QAction();
            actions.push_back(a);
            a->setText(i.value());
            this->addAction(a);
            auto t = i.key();
            QObject::connect(
                        a,
                        &QAction::triggered,
                        parent,
                        [parent, t]{ parent->createNode(t); });
        }
    }
}
