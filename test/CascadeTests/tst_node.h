#ifndef TST_NODE_H
#define TST_NODE_H

#include <memory>

#include "testheader.h"

//#include "../../src/nodegraph/node.h"
//#include "../../src/nodegraph/nodes/testdatamodel.h"

//using namespace Cascade::NodeGraph;

class NodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        //node = std::make_unique<Node>(std::make_unique<TestDataModel>());
    }
    void TearDown() override
    {

    }

    //std::unique_ptr<Node> node;

};

TEST_F(NodeTest, createSliderAndCheckReturnType)
{
//    sliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
//    sliderInt->setMinMaxStepValue(0, 100, 1, 5);

//    auto resultDouble = sliderDouble->getValue<double>();
//    auto resultInt = sliderInt->getValue<int>();

//    EXPECT_EQ(typeid(double), typeid(resultDouble));
//    EXPECT_EQ(typeid(int), typeid(resultInt));
}

#endif // TST_NODE_H
