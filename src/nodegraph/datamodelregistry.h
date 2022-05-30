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

#pragma once

#include <functional>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <QtCore/QString>

#include "nodedatamodel.h"
#include "qstringstdhash.h"
#include "memory.h"

namespace Cascade::NodeGraph
{

/// Class uses map for storing models (name, model)
class DataModelRegistry
{

public:
    using RegistryItemPtr     = std::unique_ptr<NodeDataModel>;
    using RegistryItemCreator = std::function<RegistryItemPtr()>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = std::set<QString>;

    DataModelRegistry()  = default;
    ~DataModelRegistry() = default;

    DataModelRegistry(DataModelRegistry const &) = delete;
    DataModelRegistry(DataModelRegistry &&)      = default;

    DataModelRegistry&operator=(DataModelRegistry const &) = delete;
    DataModelRegistry&operator=(DataModelRegistry &&)      = default;

public:
    template<typename ModelType>
    void registerModel(
        RegistryItemCreator creator,
        QString const &category = "Nodes")
    {
        const QString name = computeName<ModelType>(HasStaticMethodName<ModelType>{}, creator);
        if (!mRegisteredItemCreators.count(name))
        {
            mRegisteredItemCreators[name] = std::move(creator);
            mCategories.insert(category);
            mRegisteredModelsCategory[name] = category;
        }
    }

    template<typename ModelType>
    void registerModel(QString const &category = "Nodes")
    {
        RegistryItemCreator creator = [](){ return std::make_unique<ModelType>(); };
        registerModel<ModelType>(std::move(creator), category);
    }

    template<typename ModelType>
    void registerModel(
        QString const &category,
        RegistryItemCreator creator)
    {
        registerModel<ModelType>(std::move(creator), category);
    }

    template <typename ModelCreator>
    void registerModel(
        ModelCreator&& creator,
        QString const& category = "Nodes")
    {
        using ModelType = compute_model_type_t<decltype(creator())>;
        registerModel<ModelType>(std::forward<ModelCreator>(creator), category);
    }

    template <typename ModelCreator>
    void registerModel(
        QString const& category,
        ModelCreator&& creator)
    {
        registerModel(std::forward<ModelCreator>(creator), category);
    }

    std::unique_ptr<NodeDataModel>create(QString const &modelName);

    RegisteredModelCreatorsMap const &registeredModelCreators() const;

    RegisteredModelsCategoryMap const &registeredModelsCategoryAssociation() const;

    CategoriesSet const &categories() const;

private:
    RegisteredModelsCategoryMap mRegisteredModelsCategory;

    CategoriesSet mCategories;

    RegisteredModelCreatorsMap mRegisteredItemCreators;

private:
    // If the registered ModelType class has the static member method
    //
    //      static Qstring Name();
    //
    // use it. Otherwise use the non-static method:
    //
    //       virtual QString name() const;

    template <typename T, typename = void>
    struct HasStaticMethodName
        : std::false_type
    {};

    template <typename T>
    struct HasStaticMethodName<T,
                               typename std::enable_if<std::is_same<decltype(T::Name()), QString>::value>::type>
        : std::true_type
    {};

    template <typename ModelType>
    static QString computeName(std::true_type, RegistryItemCreator const&)
    {
        return ModelType::Name();
    }

    template <typename ModelType>
    static QString computeName(std::false_type, RegistryItemCreator const& creator)
    {
        return creator()->name();
    }

    template <typename T>
    struct UnwrapUniquePtr
    {
        // Assert always fires, but the compiler doesn't know this:
        static_assert(!std::is_same<T, T>::value,
                      "The ModelCreator must return a std::unique_ptr<T>, where T "
                      "inherits from NodeDataModel");
    };

    template <typename T>
    struct UnwrapUniquePtr<std::unique_ptr<T>>
    {
        static_assert(std::is_base_of<NodeDataModel, T>::value,
                      "The ModelCreator must return a std::unique_ptr<T>, where T "
                      "inherits from NodeDataModel");
        using type = T;
    };

    template <typename CreatorResult>
    using compute_model_type_t = typename UnwrapUniquePtr<CreatorResult>::type;
};

}
