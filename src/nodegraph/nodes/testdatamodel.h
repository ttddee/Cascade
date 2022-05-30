#pragma once

#include <QtCore/QObject>

#include <QtWidgets/QLabel>

#include "../nodedatamodel.h"
#include "../nodedata.h"

using Cascade::NodeGraph::NodeDataModel;
using Cascade::NodeGraph::NodeData;
using Cascade::NodeGraph::NodeDataType;
using Cascade::NodeGraph::PortType;
using Cascade::NodeGraph::PortIndex;

/// The class can potentially incapsulate any user data which
/// need to be transferred within the Node Editor graph

class TestNodeData : public NodeData
{
public:
    TestNodeData()
    {
        mCaption = "Test Node";
        mName = "TestNode";
        mInPorts = { "RGBA Back", "RGBA Front" };
        mOutPorts = { "Result" };
    }

//    NodeDataType type() const override
//    {
//        return NodeDataType {"TestNodeData", "Test Data"};
//    }
};

//class RgbaBackNodeData : public NodeData
//{
//public:
//    NodeDataType type() const override
//    {
//        return NodeDataType {"RgbaBackNodeData", "RGBA Back"};
//    }
//};

//class RgbaFrontNodeData : public NodeData
//{
//public:
//    NodeDataType type() const override
//    {
//        return NodeDataType {"RgbaFrontNodeData", "RGBA Front"};
//    }
//};

//class ResultNodeData : public NodeData
//{
//public:
//    NodeDataType type() const override
//    {
//        return NodeDataType {"ResultNodeData", "Result"};
//    }
//};

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
        unsigned int result = 1;

        switch (portType)
        {
        case PortType::In:
            result = 2;
            break;

        case PortType::Out:
            result = 1;
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

    QWidget* embeddedWidget() override { return nullptr; }
};
