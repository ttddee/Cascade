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

#include <QObject>

#include <QLabel>

#include "../../properties/intpropertymodel.h"
#include "../../properties/propertydata.h"
#include "../../properties/titlepropertymodel.h"
#include "../nodedata.h"
#include "../nodedatamodel.h"

using Cascade::Properties::IntPropertyData;
using Cascade::Properties::IntPropertyModel;
using Cascade::Properties::PropertyData;
using Cascade::Properties::TitlePropertyData;
using Cascade::Properties::TitlePropertyModel;

namespace Cascade::NodeGraph
{

class TestNodeData : public NodeData
{
public:
    TestNodeData()
    {
        mCaption = "Test Node";

        mName = "TestNode";

        mInPorts = {"RGBA Back", "RGBA Front"};

        mOutPorts = {"Result"};

        mProperties.push_back(
            std::make_unique<TitlePropertyModel>(TitlePropertyData(mCaption.toUpper())));

        mProperties.push_back(
            std::make_unique<IntPropertyModel>(IntPropertyData("Test int", 0, 100, 1, 40)));

        mProperties.push_back(
            std::make_unique<IntPropertyModel>(IntPropertyData("Test int", 0, 100, 1, 40)));
    }
};

//------------------------------------------------------------------------------

class TestNodeDataModel : public NodeDataModel
{
    Q_OBJECT

public:
    TestNodeDataModel()
    {
        mData = TestNodeData();
    }

    virtual ~TestNodeDataModel() {}
};

} // namespace Cascade::NodeGraph
