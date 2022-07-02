#ifndef TST_NODEGRAPHVIEW_H
#define TST_NODEGRAPHVIEW_H

#include "testheader.h"

//#include "../../src/nodegraph/nodegraphdatamodel.h"
//#include "../../src/nodegraph/nodegraphscene.h"
#include "../../src/nodegraph/nodegraphview.h"

using namespace Cascade::NodeGraph;

class NodeGraphViewTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mView = new NodeGraphView(&mParent);

        mModel = mView->getModel();

        mNode1 = &mModel->createNode(std::make_unique<TestNodeDataModel>());
        mNode2 = &mModel->createNode(std::make_unique<TestNodeDataModel>());
        mNode3 = &mModel->createNode(std::make_unique<TestNodeDataModel>());
    }
    void TearDown() override
    {
        mModel->removeNode(*mNode1);
        mModel->removeNode(*mNode2);
        mModel->removeNode(*mNode3);
    }

    QWidget mParent;
    NodeGraphView* mView;
    NodeGraphDataModel* mModel;

    Node* mNode1;
    Node* mNode2;
    Node* mNode3;
};

TEST_F(NodeGraphViewTest, checkThatNoNodeIsViewed)
{
    ASSERT_EQ(mNode1->getIsViewed(), false);
    ASSERT_EQ(mNode2->getIsViewed(), false);
    ASSERT_EQ(mNode3->getIsViewed(), false);
}

TEST_F(NodeGraphViewTest, checkThatNodeIsViewed)
{
    mView->setViewedNode(mNode2);

    ASSERT_EQ(mNode1->getIsViewed(), false);
    ASSERT_EQ(mNode2->getIsViewed(), true);
    ASSERT_EQ(mNode3->getIsViewed(), false);
}

TEST_F(NodeGraphViewTest, checkThatNodeIsViewedAndPreviousNodeIsNot)
{
    mView->setViewedNode(mNode2);
    mView->setViewedNode(mNode3);

    ASSERT_EQ(mNode1->getIsViewed(), false);
    ASSERT_EQ(mNode2->getIsViewed(), false);
    ASSERT_EQ(mNode3->getIsViewed(), true);
}

#endif // TST_NODEGRAPHVIEW_H
