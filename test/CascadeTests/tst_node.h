#ifndef TST_NODE_H
#define TST_NODE_H

#include <memory>

#include "testheader.h"

#include "../../src/nodegraph/node.h"
#include "../../src/nodegraph/nodes/testdatamodel.h"
#include "../../src/nodegraph/nodegraphscene.h"
#include "../../src/nodegraph/nodegraphdatamodel.h"

using namespace Cascade::NodeGraph;

class NodeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mScene = new NodeGraphScene(&mParent);
        mModel = new NodeGraphDataModel(mScene, &mParent);

        mNode1 = &mModel->createNode(std::make_unique<TestDataModel>());
        mNode2 = &mModel->createNode(std::make_unique<TestDataModel>());
        mNode3 = &mModel->createNode(std::make_unique<TestDataModel>());


    }
    void TearDown() override
    {
        mModel->removeNode(*mNode1);
        mModel->removeNode(*mNode2);
        mModel->removeNode(*mNode3);
    }

    QWidget mParent;
    NodeGraphScene* mScene;
    NodeGraphDataModel* mModel;
    Node* mNode1;
    Node* mNode2;
    Node* mNode3;

};

TEST_F(NodeTest, checkThatDataModelWasCreated)
{
    ASSERT_NE(mNode1->nodeDataModel(), nullptr);
}

TEST_F(NodeTest, checkThatPropertyWidgetGetsCreated)
{
    ASSERT_NE(mNode1->propertyWidget(), nullptr);
}

TEST_F(NodeTest, checkIsRoot)
{
    // node1 --> node2 --> node3

    mModel->createConnection(*mNode2, 0, *mNode1, 0);
    mModel->createConnection(*mNode3, 0, *mNode2, 0);

    ASSERT_EQ(mNode1->isRoot(), true);
    ASSERT_EQ(mNode2->isRoot(), false);
    ASSERT_EQ(mNode3->isRoot(), false);
}

TEST_F(NodeTest, checkIsleaf)
{
    // node1 --> node2 --> node3

    mModel->createConnection(*mNode2, 0, *mNode1, 0);
    mModel->createConnection(*mNode3, 0, *mNode2, 0);

    ASSERT_EQ(mNode1->isLeaf(), false);
    ASSERT_EQ(mNode2->isLeaf(), false);
    ASSERT_EQ(mNode3->isLeaf(), true);
}

TEST_F(NodeTest, getNodesAboveReturnsCorrectNumber)
{
    // node1 --> node2 --> node3
    //       \-----------> node3

    mModel->createConnection(*mNode2, 0, *mNode1, 0);
    mModel->createConnection(*mNode3, 0, *mNode2, 0);
    mModel->createConnection(*mNode3, 1, *mNode1, 0);

    auto nodesAboveNode3 = mNode3->getNodesAbove();
    auto nodesAboveNode2 = mNode2->getNodesAbove();
    auto nodesAboveNode1 = mNode1->getNodesAbove();

    ASSERT_EQ(nodesAboveNode3.size(), 2);
    ASSERT_EQ(nodesAboveNode2.size(), 1);
    ASSERT_EQ(nodesAboveNode1.size(), 0);
}

TEST_F(NodeTest, getNodesBelowReturnsCorrectNumber)
{
    // node1 --> node2 --> node3
    //       \-----------> node3

    mModel->createConnection(*mNode2, 0, *mNode1, 0);
    mModel->createConnection(*mNode3, 0, *mNode2, 0);
    mModel->createConnection(*mNode3, 1, *mNode1, 0);

    auto nodesBelowNode3 = mNode3->getNodesBelow();
    auto nodesBelowNode2 = mNode2->getNodesBelow();
    auto nodesBelowNode1 = mNode1->getNodesBelow();

    ASSERT_EQ(nodesBelowNode3.size(), 0);
    ASSERT_EQ(nodesBelowNode2.size(), 1);
    ASSERT_EQ(nodesBelowNode1.size(), 2);
}


#endif // TST_NODE_H
