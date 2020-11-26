#ifndef TST_CSSLIDERBOXTESTS_H
#define TST_CSSLIDERBOXTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/cssliderbox.h"

using namespace testing;

class CsSliderBoxTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sliderBoxDouble = std::unique_ptr<CsSliderBox>(new CsSliderBox(UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE));
        sliderBoxInt = std::unique_ptr<CsSliderBox>(new CsSliderBox(UI_ELEMENT_TYPE_SLIDER_BOX_INT));
    }
    void TearDown() override
    {

    }

    std::unique_ptr<CsSliderBox> sliderBoxDouble;
    std::unique_ptr<CsSliderBox> sliderBoxInt;
};

TEST_F(CsSliderBoxTest, setMinMaxStepValue_SetValuesDoubleBox)
{
    sliderBoxDouble->setMinMaxStepValue<double>(0.0, 5.0, 0.01, 1.0);

//    EXPECT_EQ(sliderBoxDouble->ui->slider->minimum(), 0);
//    EXPECT_EQ(sliderBoxDouble->ui->slider->maximum(), 500);
//    EXPECT_EQ(sliderBoxDouble->ui->slider->singleStep(), 1);
//    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 100);

//    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->minimum(), 0.0);
//    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->maximum(), 5.0);
//    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->singleStep(), 0.01);
//    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 1.0);
}

TEST_F(CsSliderBoxTest, setMinMaxStepValue_SetValuesIntBox)
{
    sliderBoxInt->setMinMaxStepValue<int>(0, 100, 1, 50);

//    EXPECT_EQ(sliderBoxInt->ui->slider->minimum(), 0);
//    EXPECT_EQ(sliderBoxInt->ui->slider->maximum(), 100);
//    EXPECT_EQ(sliderBoxInt->ui->slider->singleStep(), 1);
//    EXPECT_EQ(sliderBoxInt->ui->slider->value(), 50);

//    EXPECT_EQ(sliderBoxInt->valueBoxInt->minimum(), 0);
//    EXPECT_EQ(sliderBoxInt->valueBoxInt->maximum(), 100);
//    EXPECT_EQ(sliderBoxInt->valueBoxInt->singleStep(), 1);
//    EXPECT_EQ(sliderBoxInt->valueBoxInt->value(), 50);
}

#endif // TST_CSSLIDERBOXTESTS_H
