#ifndef TST_NODE_H
#define TST_NODE_H

#include <memory>

#include "testheader.h"

#include "../../src/nodegraph/node.h"
#include "../../src/nodegraph/nodes/testdatamodel.h"

using namespace Cascade::NodeGraph;

class NodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mNode = std::make_unique<Node>(std::make_unique<TestDataModel>());
    }
    void TearDown() override
    {

    }

    std::unique_ptr<Node> mNode;

};

TEST_F(NodeTest, checkThatDataModelWasCreated)
{
    ASSERT_NE(mNode->nodeDataModel(), nullptr);
}

TEST_F(NodeTest, checkThatPropertyWidgetGetsCreated)
{
    ASSERT_NE(mNode->propertyWidget(), nullptr);
}

#endif // TST_NODE_H
