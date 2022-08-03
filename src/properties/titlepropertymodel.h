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

#ifndef TITLEPROPERTYMODEL_H
#define TITLEPROPERTYMODEL_H

#include "propertymodel.h"
#include "titlepropertyview.h"

using Cascade::Properties::TitlePropertyView;

namespace Cascade::Properties
{

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

} // namespace Cascade::Properties

#endif // TITLEPROPERTYMODEL_H
