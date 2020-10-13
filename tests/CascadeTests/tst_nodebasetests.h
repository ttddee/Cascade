#ifndef TST_NODEBASETESTS_H
#define TST_NODEBASETESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/nodebase.h"
#include "../../src/nodegraph.h"
#include "../../src/nodedefinitions.h"

using namespace testing;

class NodeBaseTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        nodeGraph = new NodeGraph(nullptr);
        node = new NodeBase(NODE_TYPE_MERGE, nodeGraph);
    }

    void TearDown() override
    {
        delete node;
    }

    NodeGraph* nodeGraph;
    NodeBase* node;
};

TEST_F(NodeBaseTest, firstTest)
{
    //auto s = node->getTargetSize();

    EXPECT_EQ(1, 0);
}

#endif // TST_NODEBASETESTS_H
