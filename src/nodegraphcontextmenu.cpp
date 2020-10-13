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
