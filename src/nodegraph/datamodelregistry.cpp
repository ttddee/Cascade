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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#include "datamodelregistry.h"

#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>

using Cascade::NodeGraph::DataModelRegistry;
using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeDataType;

std::unique_ptr<NodeDataModel> DataModelRegistry::create(QString const &modelName)
{
    auto it = mRegisteredItemCreators.find(modelName);

    if (it != mRegisteredItemCreators.end())
    {
        return it->second();
    }

    return nullptr;
}


DataModelRegistry::RegisteredModelCreatorsMap const& DataModelRegistry::registeredModelCreators() const
{
    return mRegisteredItemCreators;
}


DataModelRegistry::RegisteredModelsCategoryMap const& DataModelRegistry::registeredModelsCategoryAssociation() const
{
    return mRegisteredModelsCategory;
}


DataModelRegistry::CategoriesSet const& DataModelRegistry::categories() const
{
    return mCategories;
}

