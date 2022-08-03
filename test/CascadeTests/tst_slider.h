#ifndef TST_SLIDER_H
#define TST_SLIDER_H

#include "testheader.h"

#include "../../src/ui/slider.h"

using Cascade::UiElements::Slider;
using Cascade::UiElements::SliderType;

class SliderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        mSliderDouble = new Slider(SliderType::Double, &mParent);
        mSliderInt    = new Slider(SliderType::Int, &mParent);
    }

    void TearDown() override {}

    QWidget mParent;
    Slider* mSliderDouble;
    Slider* mSliderInt;
};

TEST_F(SliderTest, createSliderAndCheckReturnType)
{
    mSliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
    mSliderInt->setMinMaxStepValue(0, 100, 1, 5);

    auto resultDouble = mSliderDouble->getValue<double>();
    auto resultInt    = mSliderInt->getValue<int>();

    EXPECT_EQ(typeid(double), typeid(resultDouble));
    EXPECT_EQ(typeid(int), typeid(resultInt));
}

TEST_F(SliderTest, createSliderAndCheckReturnValue)
{
    mSliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
    mSliderInt->setMinMaxStepValue(0, 100, 1, 5);

    auto resultDouble = mSliderDouble->getValue<double>();
    auto resultInt    = mSliderInt->getValue<int>();

    EXPECT_EQ(resultDouble, 0.5);
    EXPECT_EQ(resultInt, 5);
}

#endif // TST_SLIDER_H
