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

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>

#include "propertydata.h"
#include "../properties/propertyview.h"

using Cascade::Properties::PropertyView;
using Cascade::Properties::IntPropertyView;

namespace Cascade::NodeGraph
{

class Property : public QObject
{
    Q_OBJECT

public:
    virtual PropertyView* getView() const = 0;

private:
    PropertyView* mView = nullptr;

};

class IntProperty : public Property
{
    Q_OBJECT

public:
    IntProperty(IntPropertyData data)
        : mData(data)
    {
        mView = new IntPropertyView();
    }

    PropertyView* getView() const override
    {
        return mView;
    };

private:
    IntPropertyData mData;
    IntPropertyView* mView;

};

} // namespace Cascade::NodeGraph

#endif // PROPERTY_H
