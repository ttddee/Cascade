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

TEST_F(NodeGraphDataModelTest, checkThatDataWasCreated)
{
    ASSERT_NE(mModel->getData(), nullptr);
}

#endif // TST_NODEGRAPHDATAMODEL_H
