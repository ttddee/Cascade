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

