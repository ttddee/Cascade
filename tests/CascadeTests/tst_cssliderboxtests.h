#ifndef TST_CSSLIDERBOXTESTS_H
#define TST_CSSLIDERBOXTESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../../src/cssliderbox.h"

namespace Ui {
class CsSliderBox;
}

#include "ui_cssliderbox.h"

using namespace testing;

class CsSliderBoxTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sliderBoxDouble = std::unique_ptr<CsSliderBox>(new CsSliderBox(UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE));
        sliderBoxDouble = std::unique_ptr<CsSliderBox>(new CsSliderBox(UI_ELEMENT_TYPE_SLIDER_BOX_INT));
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

    EXPECT_EQ(sliderBoxDouble->ui->slider->minimum(), 0);
    EXPECT_EQ(sliderBoxDouble->ui->slider->maximum(), 500);
    EXPECT_EQ(sliderBoxDouble->ui->slider->singleStep(), 1);
    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 100);

    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->minimum(), 0);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->maximum(), 500);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->singleStep(), 1);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 100);
}

#endif // TST_CSSLIDERBOXTESTS_H
