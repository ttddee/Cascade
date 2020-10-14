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
        /*
         * | Read1 | --- | Color | --- | Merge | --- | Write |
         *                            /
         * | Read2 | --- | Resize | -
        */

        nodeGraph = new NodeGraph(nullptr);
        readNode1 = new NodeBase(NODE_TYPE_READ, nodeGraph);
        readNode2 = new NodeBase(NODE_TYPE_READ, nodeGraph);
        colorNode= new NodeBase(NODE_TYPE_COLOR, nodeGraph);
        resizeNode= new NodeBase(NODE_TYPE_RESIZE, nodeGraph);
        mergeNode= new NodeBase(NODE_TYPE_MERGE, nodeGraph);
        writeNode= new NodeBase(NODE_TYPE_WRITE, nodeGraph);

        auto c = nodeGraph->createOpenConnection(readNode1->getRgbaOut());
        connections.push_back(c);
        nodeGraph->establishConnection(colorNode->getRgbaBackIn());

        c = nodeGraph->createOpenConnection(colorNode->getRgbaOut());
        connections.push_back(c);
        nodeGraph->establishConnection(mergeNode->getRgbaBackIn());

        c = nodeGraph->createOpenConnection(mergeNode->getRgbaOut());
        connections.push_back(c);
        nodeGraph->establishConnection(writeNode->getRgbaBackIn());

        c = nodeGraph->createOpenConnection(readNode2->getRgbaOut());
        connections.push_back(c);
        nodeGraph->establishConnection(resizeNode->getRgbaBackIn());

        c = nodeGraph->createOpenConnection(resizeNode->getRgbaOut());
        connections.push_back(c);
        nodeGraph->establishConnection(mergeNode->getRgbaFrontIn());
    }

    void TearDown() override
    {
        foreach(auto c, connections)
        {
            delete c;
        }
        delete readNode1;
        delete readNode2;
        delete colorNode;
        delete resizeNode;
        delete mergeNode;
        delete writeNode;
        delete nodeGraph;
    }

    NodeGraph* nodeGraph;
    NodeBase* readNode1;
    NodeBase* readNode2;
    NodeBase* colorNode;
    NodeBase* resizeNode;
    NodeBase* mergeNode;
    NodeBase* writeNode;
    std::vector<Connection*> connections;

};

TEST_F(NodeBaseTest, getAllConnections_NodesConnected_NumberConnections)
{
    auto connsRead = readNode1->getAllConnections();
    auto connsMerge = mergeNode->getAllConnections();
    auto connsResize = resizeNode->getAllConnections();
    auto connsWrite = writeNode->getAllConnections();

    EXPECT_EQ(connsRead.size(), 1);
    EXPECT_EQ(connsMerge.size(), 3);
    EXPECT_EQ(connsResize.size(), 2);
    EXPECT_EQ(connsWrite.size(), 1);
}

#endif // TST_NODEBASETESTS_H
