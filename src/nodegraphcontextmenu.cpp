#include "nodegraphcontextmenu.h"

#include <iostream>

#include "nodedefinitions.h"
#include "nodegraph.h"

using namespace Cascade;

NodeGraphContextMenu::NodeGraphContextMenu(NodeGraph* parent)
    : QMenu(parent)
{
    QMapIterator<NodeType, QString> i(nodeStrings);
    while (i.hasNext())
    {
        i.next();
        auto a = new QAction();
        actions.push_back(a);
        a->setText(i.value());
        this->addAction(a);
        auto nodePos = parent->mapFromGlobal(QCursor::pos());
        auto t = i.key();
        QObject::connect(
                    a,
                    &QAction::triggered,
                    parent,
                    [parent, t, nodePos]{ parent->createNode(t, nodePos); });
    }
}
