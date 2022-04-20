/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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

#include "nodecontextmenu.h"

#include "nodegraph.h"

namespace Cascade {

NodeContextMenu::NodeContextMenu(
        NodeGraph* nodeGraph,
        NodeBase* node)
    : QMenu(nodeGraph)
{
    auto a = new QAction();
    mActions.push_back(a);
    a->setText("View");
    this->addAction(a);
    QObject::connect(
                a,
                &QAction::triggered,
                this,
                [this, node]{ emit requestNodeDisplay(node); });

    connect(this, &NodeContextMenu::requestNodeDisplay,
            nodeGraph, &NodeGraph::handleNodeDisplayRequest);

//    a = new QAction();
//    actions.push_back(a);
//    a->setText("Delete");
//    this->addAction(a);
//    QObject::connect(
//                a,
//                &QAction::triggered,
//                parent,
//                [parent, node]{ parent->deleteNode(node); });
}

NodeContextMenu::~NodeContextMenu()
{
    foreach (auto& action, mActions)
    {
        delete action;
    }
}

} // namespace Cascade
