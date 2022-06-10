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
        //sliderDouble =  new Slider(SliderType::Double, &parent);//std::make_unique<Slider>(SliderType::Double);
        //sliderInt =     new Slider(SliderType::Int, &parent);//std::make_unique<Slider>(SliderType::Int);
    }
    void TearDown() override
    {

    }

    //QWidget parent;
    //Slider* sliderDouble;
    //Slider* sliderInt;
};

TEST_F(SliderTest, createSliderAndCheckReturnType)
{
//    sliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
//    sliderInt->setMinMaxStepValue(0, 100, 1, 5);

//    auto resultDouble = sliderDouble->getValue<double>();
//    auto resultInt = sliderInt->getValue<int>();

//    EXPECT_EQ(typeid(double), typeid(resultDouble));
//    EXPECT_EQ(typeid(int), typeid(resultInt));
}

TEST_F(SliderTest, createSliderAndCheckReturnValue)
{
//    sliderDouble->setMinMaxStepValue(0.0, 1.0, 0.1, 0.5);
//    sliderInt->setMinMaxStepValue(0, 100, 1, 5);

//    auto resultDouble = sliderDouble->getValue<double>();
//    auto resultInt = sliderInt->getValue<int>();

//    EXPECT_EQ(resultDouble, 0.5);
//    EXPECT_EQ(resultInt, 5);
}

#endif // TST_SLIDER_H
