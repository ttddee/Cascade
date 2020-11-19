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
#include "gmichelper.h"
#include "gmic/FiltersModel.h"

using namespace Cascade;

NodeGraphContextMenu::NodeGraphContextMenu(NodeGraph* parent)
    : QMenu(parent)
{
    this->setFixedWidth(150);

    // Populate menu with submenus aka categories
    GmicHelper& gmicHelper = GmicHelper::getInstance();

    QMap<NodeCategory, QMenu*> categories;
    QMap<QString, QMenu*> gmicCategories;
    QSet<QString> filterCategories;


    auto filtersModel = gmicHelper.getFiltersModel();
    {
        QMapIterator<NodeCategory, QString> i(categoryStrings);
        while (i.hasNext())
        {
            i.next();
            categories[i.key()] = this->addMenu(categoryStrings[i.key()]);
            if (i.key() == NODE_CATEGORY_GMIC)
            {
                std::vector<QString> sortingCategories;

                GmicHelper& gmicHelper = GmicHelper::getInstance();
                filterCategories = gmicHelper.getFilterCategories();

                QSet<QString>::iterator j = filterCategories.begin();
                while (j != filterCategories.end())
                {
                    sortingCategories.push_back(*j);
                    j++;
                }
                std::sort(sortingCategories.begin(), sortingCategories.end());
                foreach(auto& cat, sortingCategories)
                {
                    gmicCategories[cat] = categories[NODE_CATEGORY_GMIC]->addMenu(cat);
                }
            }
        }
    }

    // Add nodes to corresponding submenus
    QMapIterator<NodeType, QString> i(nodeStrings);
    while (i.hasNext())
    {
        i.next();
        if (i.key() == NODE_TYPE_GMIC)
        {
            std::vector<std::pair<QString, QString>> sortingFilters;
            for (const FiltersModel::Filter& filter : filtersModel)
            {
                sortingFilters.push_back(std::pair(filter.name(), filter.hash()));
            }
            std::sort(sortingFilters.begin(), sortingFilters.end());

            foreach (auto& filter, sortingFilters)
            {
                auto a = new QAction();
                actions.push_back(a);
                a->setText(filter.first);
                auto t = NODE_TYPE_GMIC;
                gmicCategories[filtersModel.getFilterCategoryFromHash(filter.second)]->addAction(a);

                QObject::connect(
                            a,
                            &QAction::triggered,
                            parent,
                            [parent, t, filter]
                            { parent->createNode(t, filter.first, filter.second); });
              }
        }
        else
        {

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
}
