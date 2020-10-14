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

TEST_F(NodeBaseTest, getAllConnections_CorrectNumberOfConnections)
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

TEST_F(NodeBaseTest, getAllUpstreamNodes_CorrectNumberOfNodes)
{
    std::vector<NodeBase*> nodes;
    writeNode->getAllUpstreamNodes(nodes);

    EXPECT_EQ(nodes.size(), 6);
}

TEST_F(NodeBaseTest, getAllUpstreamNodes_CorrectOrderOfNodes)
{
    std::vector<NodeBase*> nodes;
    writeNode->getAllUpstreamNodes(nodes);

    EXPECT_EQ(nodes[0]->getID(), readNode1->getID());
    EXPECT_EQ(nodes[1]->getID(), colorNode->getID());
    EXPECT_EQ(nodes[2]->getID(), readNode2->getID());
    EXPECT_EQ(nodes[3]->getID(), resizeNode->getID());
    EXPECT_EQ(nodes[4]->getID(), mergeNode->getID());
    EXPECT_EQ(nodes[5]->getID(), writeNode->getID());
}

TEST_F(NodeBaseTest, getAllDownstreamNodes_CorrectNumberOfNodes)
{
    std::vector<NodeBase*> nodes;
    readNode1->getAllDownstreamNodes(nodes);

    EXPECT_EQ(nodes.size(), 3);
}

TEST_F(NodeBaseTest, getAllDownstreamNodes_CorrectOrderOfNodes)
{
    std::vector<NodeBase*> nodes;
    readNode1->getAllDownstreamNodes(nodes);

    EXPECT_EQ(nodes[0]->getID(), colorNode->getID());
    EXPECT_EQ(nodes[1]->getID(), mergeNode->getID());
    EXPECT_EQ(nodes[2]->getID(), writeNode->getID());
}

TEST_F(NodeBaseTest, invalidateAllDownstreamNodes_AllInvalidated)
{
    readNode1->needsUpdate = false;
    colorNode->needsUpdate = false;
    readNode2->needsUpdate = false;
    resizeNode->needsUpdate = false;
    mergeNode->needsUpdate = false;
    writeNode->needsUpdate = false;

    readNode1->invalidateAllDownstreamNodes();

    EXPECT_EQ(colorNode->needsUpdate, true);
    EXPECT_EQ(mergeNode->needsUpdate, true);
    EXPECT_EQ(writeNode->needsUpdate, true);
    EXPECT_EQ(readNode1->needsUpdate, false);
    EXPECT_EQ(readNode2->needsUpdate, false);
    EXPECT_EQ(resizeNode->needsUpdate, false);

}

#endif // TST_NODEBASETESTS_H
