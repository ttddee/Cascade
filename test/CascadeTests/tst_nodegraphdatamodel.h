#ifndef TST_NODEGRAPHDATAMODEL_H
#define TST_NODEGRAPHDATAMODEL_H

#include "testheader.h"

#include "../../src/nodegraph/nodegraphscene.h"
#include "../../src/nodegraph/nodegraphdatamodel.h"

using namespace Cascade::NodeGraph;

class NodeGraphDataModelTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mScene = new NodeGraphScene(&mParent);
        mModel = new NodeGraphDataModel(mScene, &mParent);
    }
    void TearDown() override
    {

    }

    QWidget mParent;
    NodeGraphScene* mScene;
    NodeGraphDataModel* mModel;
};

TEST_F(NodeGraphDataModelTest, checkThatDataGetsCreated)
{
    ASSERT_NE(mModel->getData(), nullptr);
}

TEST_F(NodeGraphDataModelTest, checkIsRoot)
{
//    auto& node1 = mModel->createNode(std::make_unique<TestDataModel>());
//    auto& node2 = mModel->createNode(std::make_unique<TestDataModel>());

//    mModel->createConnection(node1, 0, node2, 0);

//    ASSERT_EQ(node1.isRoot(), true);
//    ASSERT_EQ(node2.isRoot(), false);
}

#endif // TST_NODEGRAPHDATAMODEL_H
