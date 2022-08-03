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

#ifndef FILESPROPERTYMODEL_H
#define FILESPROPERTYMODEL_H

#include "filespropertyview.h"
#include "propertymodel.h"

using Cascade::Properties::PropertyView;

namespace Cascade::Properties
{

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

    void removeEntry(const int index)
    {
        mData->getFiles()->removeRows(index, 1);
    }

    int numEntries()
    {
        return mData->getFiles()->rowCount();
    }

private:
    std::unique_ptr<FilesPropertyData> mData;
    FilesPropertyView* mView;
};

} // namespace Cascade::Properties

#endif // FILESPROPERTYMODEL_H
