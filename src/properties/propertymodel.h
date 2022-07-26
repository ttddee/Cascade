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

#ifndef PROPERTYMODEL_H
#define PROPERTYMODEL_H

#include <memory>

#include <QObject>

#include "../properties/filespropertyview.h"
#include "../properties/intpropertyview.h"
#include "../properties/propertyview.h"
#include "../properties/titlepropertyview.h"
#include "propertydata.h"

using Cascade::Properties::IntPropertyView;
using Cascade::Properties::PropertyView;
using Cascade::Properties::TitlePropertyView;

namespace Cascade::Properties
{

class PropertyModel : public QObject
{
    Q_OBJECT

public:
    virtual PropertyData* getData() = 0;
    virtual PropertyView* getView() = 0;
};

class TitlePropertyModel : public PropertyModel
{
    Q_OBJECT

public:
    TitlePropertyModel(TitlePropertyData data)
        : mData(std::make_unique<TitlePropertyData>(data))
    {
        mView = new TitlePropertyView();
        mView->setModel(this);
    }

    TitlePropertyData* getData() override
    {
        return mData.get();
    };

    TitlePropertyView* getView() override
    {
        return mView;
    };

private:
    std::unique_ptr<TitlePropertyData> mData;
    TitlePropertyView* mView;
};

class IntPropertyModel : public PropertyModel
{
    Q_OBJECT

public:
    IntPropertyModel(IntPropertyData data)
        : mData(std::make_unique<IntPropertyData>(data))
    {
        mView = new IntPropertyView();
        mView->setModel(this);
    }

    IntPropertyData* getData() override
    {
        return mData.get();
    };

    IntPropertyView* getView() override
    {
        return mView;
    };

    void setValue(const int value)
    {
        mData->setValue(value);
    }

private:
    std::unique_ptr<IntPropertyData> mData;
    IntPropertyView* mView;
};

class FilesPropertyModel : public PropertyModel
{
    Q_OBJECT

public:
    FilesPropertyModel(FilesPropertyData data)
        : mData(std::make_unique<FilesPropertyData>(data))
    {
        mView = new FilesPropertyView();
        mView->setModel(this);
    }

    FilesPropertyData* getData() override
    {
        return mData.get();
    };

    FilesPropertyView* getView() override
    {
        return mView;
    };

    void addEntries(const QStringList& entries)
    {
        mData->append(entries);
    }

private:
    std::unique_ptr<FilesPropertyData> mData;
    FilesPropertyView* mView;
};

} // namespace Cascade::Properties

#endif // PROPERTYMODEL_H
