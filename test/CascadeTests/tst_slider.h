#ifndef TST_SLIDER_H
#define TST_SLIDER_H

#include "testheader.h"

#include "../../src/ui/slider.h"

using namespace Cascade::UiElements;

class SliderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sliderDouble =  std::make_unique<Slider>(SliderType::Double);
        sliderInt =     std::make_unique<Slider>(SliderType::Int);
    }
    void TearDown() override
    {

    }

    std::unique_ptr<Slider> sliderDouble;
    std::unique_ptr<Slider> sliderInt;
};

TEST_F(SliderTest, createSliderAndCheckReturnType)
{
    sliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
    sliderInt->setMinMaxStepValue(0, 100, 1, 5);

    auto resultDouble = sliderDouble->getValue<double>();
    auto resultInt = sliderInt->getValue<int>();

    EXPECT_EQ(typeid(double), typeid(resultDouble));
    EXPECT_EQ(typeid(int), typeid(resultInt));
}

TEST_F(SliderTest, createSliderAndCheckReturnValue)
{
    sliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
    sliderInt->setMinMaxStepValue(0, 100, 1, 5);

    auto resultDouble = sliderDouble->getValue<double>();
    auto resultInt = sliderInt->getValue<int>();

    EXPECT_EQ(resultDouble, 0.5);
    EXPECT_EQ(resultInt, 5);
}

#endif // TST_SLIDER_H
