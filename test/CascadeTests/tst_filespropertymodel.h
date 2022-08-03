#ifndef TST_FILESPROPERTYMODEL_H
#define TST_FILESPROPERTYMODEL_H

#include "testheader.h"

#include "../../src/properties/filespropertymodel.h"

using Cascade::Properties::FilesPropertyData;
using Cascade::Properties::FilesPropertyModel;

class FilesPropertyModelTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mModel = std::make_unique<FilesPropertyModel>(FilesPropertyData());
    }

    void TearDown() override {}

    std::unique_ptr<FilesPropertyModel> mModel;
};

TEST_F(FilesPropertyModelTest, addEntriesAndCheckCount)
{
    QStringList entries =
    {
        "/this/is/path/1",
        "/this/is/path/2",
        "/this/is/path/3"
    };

    mModel->addEntries(entries);
    mModel->addEntries(entries);

    EXPECT_EQ(6, mModel->numEntries());
}

TEST_F(FilesPropertyModelTest, removeEntryAndCheckCount)
{
    QStringList entries =
    {
        "/this/is/path/1",
        "/this/is/path/2",
        "/this/is/path/3",
        "/this/is/path/4",
        "/this/is/path/5",
        "/this/is/path/6"
    };

    mModel->addEntries(entries);

    EXPECT_EQ(6, mModel->numEntries());

    mModel->removeEntry(3);

    EXPECT_EQ(5, mModel->numEntries());
}

#endif // TST_FILESPROPERTYMODEL_H
