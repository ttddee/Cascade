#ifndef TST_RESIZEPROPERTIESENTITYTESTS_H
#define TST_RESIZEPROPERTIESENTITYTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/uientities/resizepropertiesentity.h"
#include "../../src/nodedefinitions.h"

using namespace testing;

namespace Ui {
class ResizePropertiesEntity;
}

class ResizePropertiesEntityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        entity = new ResizePropertiesEntity(
                    UI_ELEMENT_TYPE_RESIZE_PROPERTIES);
    }

    void TearDown() override
    {
        delete entity;
    }

    ResizePropertiesEntity* entity;

};

TEST_F(ResizePropertiesEntityTest, getValuesAsString_CorrectInitialValues)
{
    entity->setInputSize(QSize(1000, 1000));

    QString s = entity->getValuesAsString();

    EXPECT_EQ(s.toStdString(), "1000,1000");
}

TEST_F(ResizePropertiesEntityTest, setWidthPercentLinked_CorrectPixelValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(true);

    entity->setWidthPercent(50);

    QString s = entity->getValuesAsString();

    EXPECT_EQ(s.toStdString(), "500,500");
}

TEST_F(ResizePropertiesEntityTest, setHeightPercentLinked_CorrectPixelValues)
{
    entity->setInputSize(QSize(1000, 500));

    entity->setLinked(true);

    entity->setHeightPercent(50);

    QString s = entity->getValuesAsString();

    EXPECT_EQ(s.toStdString(), "500,250");
}

TEST_F(ResizePropertiesEntityTest, setWidthPercenNotLinked_CorrectPixelValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(false);

    entity->setWidthPercent(50);
    entity->setHeightPercent(70);

    QString s = entity->getValuesAsString();

    EXPECT_EQ(s.toStdString(), "500,700");
}
TEST_F(ResizePropertiesEntityTest, setHeightPercenNotLinked_CorrectPixelValues)
{
    entity->setInputSize(QSize(500, 1000));

    entity->setLinked(false);

    entity->setWidthPercent(50);
    entity->setHeightPercent(68);

    QString s = entity->getValuesAsString();

    EXPECT_EQ(s.toStdString(), "250,680");
}

TEST_F(ResizePropertiesEntityTest, setWidthPixelsLinked_CorrectPercentValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(true);

    entity->setWidthPixels(500);

    EXPECT_EQ(entity->getWidthPercent(), 50);
    EXPECT_EQ(entity->getHeightPercent(), 50);
}

TEST_F(ResizePropertiesEntityTest, setHeightPixelsLinked_CorrectPercentValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(true);

    entity->setHeightPixels(300);

    EXPECT_EQ(entity->getWidthPercent(), 30);
    EXPECT_EQ(entity->getHeightPercent(), 30);
}

TEST_F(ResizePropertiesEntityTest, setWidthPixelsNotLinked_CorrectPercentValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(false);

    entity->setWidthPixels(700);

    EXPECT_EQ(entity->getWidthPercent(), 70);
    EXPECT_EQ(entity->getHeightPercent(), 100);
}

TEST_F(ResizePropertiesEntityTest, setHeightPixelsNotLinked_CorrectPercentValues)
{
    entity->setInputSize(QSize(1000, 1000));

    entity->setLinked(false);

    entity->setHeightPixels(600);

    EXPECT_EQ(entity->getWidthPercent(), 100);
    EXPECT_EQ(entity->getHeightPercent(), 60);
}


#endif // TST_RESIZEPROPERTIESENTITYTESTS_H
