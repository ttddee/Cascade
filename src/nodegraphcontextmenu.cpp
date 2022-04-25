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

#include "nodegraphcontextmenu.h"

#include "nodedefinitions.h"
#include "log.h"
#include "isfmanager.h"
#include "nodegraph.h"

namespace Cascade {

NodeGraphContextMenu::NodeGraphContextMenu(NodeGraph* parent)
    : QMenu(parent)
{
    // TODO: This is doubled in mainmenu

    // Populate menu with submenus aka categories
    QMap<NodeCategory, QMenu*> categories;
    {
        QMapIterator<NodeCategory, QString> i(categoryStrings);
        while (i.hasNext())
        {
            i.next();
            auto submenu = this->addMenu(categoryStrings[i.key()]);
            categories[i.key()] = submenu;
        }
    }

    // Add nodes to corresponding submenus
    auto nodes = nodeStrings;
    nodes.remove(NodeType::eIsf);
    QMapIterator<NodeType, QString> i(nodes);
    while (i.hasNext())
    {
        i.next();
        auto a = new QAction();
        mActions.push_back(a);
        a->setText(i.value());
        auto t = i.key();
        categories[getPropertiesForType(t).category]->addAction(a);

        QObject::connect(
                    a,
                    &QAction::triggered,
                    this,
                    [this, t]{ emit requestNodeCreation(
                        t, NodeGraphPosition::eAtCursor); });
    }

    // Add ISF categories
    auto isfManager = &ISFManager::getInstance();
    std::set<QString> isfCategoryStrings = isfManager->getCategories();

    QMap<QString, QMenu*> isfCategories;
    {
        for (auto& cat : isfCategoryStrings)
        {
            auto submenu = categories.value(NodeCategory::eIsf)->addMenu(cat);
            isfCategories[cat] = submenu;
        }
    }

    // Add ISF nodes
    auto isfNodeProperties = isfManager->getNodeProperties();
    for (auto& prop : isfNodeProperties)
    {
        QString nodeName = prop.second.title;
        auto a = new QAction();
        mActions.push_back(a);
        a->setText(nodeName);
        auto t = NodeType::eIsf;
        isfCategories[isfManager->getCategoryPerNode(nodeName)]->addAction(a);

        QObject::connect(
                    a,
                    &QAction::triggered,
                    this,
                    [this, t, nodeName]{ emit requestNodeCreation(
                        t,
                        NodeGraphPosition::eAtCursor,
                        nodeName); });
    }
    connect(this, &NodeGraphContextMenu::requestNodeCreation,
            parent, &NodeGraph::handleNodeCreationRequest);
}

NodeGraphContextMenu::~NodeGraphContextMenu()
{
    foreach (auto& action, mActions)
    {
        delete action;
    }
}

} // namespace Cascade
