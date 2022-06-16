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

#include <QtCore/QObject>

#include <QtWidgets/QLabel>

#include "../nodedatamodel.h"
#include "../nodedata.h"
#include "../../properties/propertymodel.h"
#include "../../properties/propertydata.h"

using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeData;
using Cascade::NodeGraph::NodeDataType;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;
using Cascade::Properties::IntPropertyModel;
using Cascade::Properties::PropertyModel;
using Cascade::Properties::PropertyData;
using Cascade::Properties::IntPropertyData;
using Cascade::Properties::TitlePropertyData;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph

class TestNodeData : public NodeData
{
public:
    TestNodeData()
    {
        mCaption =      "Test Node";
        mName =         "TestNode";
        mInPorts =      { "RGBA Back", "RGBA Front" };
        mOutPorts =     { "Result" };
        mProperties.push_back(
            std::unique_ptr<PropertyModel>(new TitlePropertyModel(
                TitlePropertyData(mCaption.toUpper()))));
        mProperties.push_back(
            std::unique_ptr<PropertyModel>(new IntPropertyModel(
                IntPropertyData("Test int", 0, 100, 1, 40))));
        mProperties.push_back(
            std::unique_ptr<PropertyModel>(new IntPropertyModel(
                IntPropertyData("Test int", 0, 100, 1, 40))));

    }

//    NodeDataType type() const override
//    {
//        return NodeDataType {"TestNodeData", "Test Data"};
//    }
};

//------------------------------------------------------------------------------

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class TestDataModel : public NodeDataModel
{
    Q_OBJECT

public:
    TestDataModel()
    {
        mData = TestNodeData();
    }

    virtual ~TestDataModel() {}

public:
    unsigned int nPorts(PortType portType) const override
    {
        int result = 1;

        switch (portType)
        {
        case PortType::In:
            result = static_cast<unsigned int>(mData.mInPorts.size());
            break;

        case PortType::Out:
            result = static_cast<unsigned int>(mData.mOutPorts.size());
            break;
        case PortType::None:
            break;
        }

        return result;
    }

    NodeDataType dataType(
        PortType portType,
        PortIndex portIndex) const override
    {
//        switch (portType)
//        {
//        case PortType::In:
//            switch (portIndex)
//            {
//            case 0:
//                return RgbaBackNodeData().type();
//            case 1:
//                return RgbaFrontNodeData().type();
//            }
//            break;

//        case PortType::Out:
//            switch (portIndex)
//            {
//            case 0:
//                return ResultNodeData().type();
//            }
//            break;

//        case PortType::None:
//            break;
//        }
        // FIXME: control may reach end of non-void function [-Wreturn-type]
        return NodeDataType();
    }

    std::shared_ptr<NodeData> outData(PortIndex port) override
    {
        return std::make_shared<NodeData>();
    }

    void setInData(std::shared_ptr<NodeData>, int) override
    {
        //
    }

    //QWidget* embeddedWidget() override { return nullptr; }
};
