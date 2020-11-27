#ifndef TST_CSSLIDERBOXTESTS_H
#define TST_CSSLIDERBOXTESTS_H

#include <QSignalSpy>

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
    sliderBoxDouble->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);

    EXPECT_EQ(sliderBoxDouble->ui->slider->minimum(), 0);
    EXPECT_EQ(sliderBoxDouble->ui->slider->maximum(), 500);
    EXPECT_EQ(sliderBoxDouble->ui->slider->singleStep(), 1);
    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 100);

    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->minimum(), 0.0);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->maximum(), 5.0);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->singleStep(), 0.01);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 1.0);
}

TEST_F(CsSliderBoxTest, setMinMaxStepValue_SetValuesIntBox)
{
    sliderBoxInt->setMinMaxStepValue(0, 100, 1, 50);

    EXPECT_EQ(sliderBoxInt->ui->slider->minimum(), 0);
    EXPECT_EQ(sliderBoxInt->ui->slider->maximum(), 100);
    EXPECT_EQ(sliderBoxInt->ui->slider->singleStep(), 1);
    EXPECT_EQ(sliderBoxInt->ui->slider->value(), 50);

    EXPECT_EQ(sliderBoxInt->valueBoxInt->minimum(), 0);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->maximum(), 100);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->singleStep(), 1);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->value(), 50);
}

TEST_F(CsSliderBoxTest, setSliderValue_DoubleSpinBoxHasCorrectValue)
{
    QSignalSpy spy(&(*sliderBoxDouble), SIGNAL(valueChanged()));

    sliderBoxDouble->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);

    sliderBoxDouble->ui->slider->setValue(450);

    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 450);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 4.5);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CsSliderBoxTest, setSliderValue_IntSpinBoxHasCorrectValue)
{
    QSignalSpy spy(&(*sliderBoxInt), SIGNAL(valueChanged()));

    sliderBoxInt->setMinMaxStepValue(0, 100, 1, 50);

    sliderBoxInt->ui->slider->setValue(35);

    EXPECT_EQ(sliderBoxInt->ui->slider->value(), 35);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->value(), 35);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CsSliderBoxTest, setDoubleSpinBoxValue_SliderHasCorrectValue)
{
    QSignalSpy spy(&(*sliderBoxDouble), SIGNAL(valueChanged()));

    sliderBoxDouble->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);

    sliderBoxDouble->valueBoxDouble->setValue(3.5);

    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 350);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 3.5);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CsSliderBoxTest, setIntSpinBoxValue_SliderHasCorrectValue)
{
    QSignalSpy spy(&(*sliderBoxInt), SIGNAL(valueChanged()));

    sliderBoxInt->setMinMaxStepValue(0, 200, 1, 70);

    sliderBoxInt->ui->slider->setValue(75);

    EXPECT_EQ(sliderBoxInt->ui->slider->value(), 75);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->value(), 75);
    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CsSliderBoxTest, resetDouble_AllValuesCorrect)
{
    QSignalSpy spy(&(*sliderBoxDouble), SIGNAL(valueChanged()));

    sliderBoxDouble->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);

    sliderBoxDouble->valueBoxDouble->setValue(3.5);

    sliderBoxDouble->reset();

    EXPECT_EQ(sliderBoxDouble->ui->slider->value(), 100);
    EXPECT_EQ(sliderBoxDouble->valueBoxDouble->value(), 1.0);
    EXPECT_EQ(spy.count(), 2);
}

TEST_F(CsSliderBoxTest, resetInt_AllValuesCorrect)
{
    QSignalSpy spy(&(*sliderBoxInt), SIGNAL(valueChanged()));

    sliderBoxInt->setMinMaxStepValue(0, 300, 1, 112);

    sliderBoxInt->ui->slider->setValue(56);

    sliderBoxInt->reset();

    EXPECT_EQ(sliderBoxInt->ui->slider->value(), 112);
    EXPECT_EQ(sliderBoxInt->valueBoxInt->value(), 112);
    EXPECT_EQ(spy.count(), 2);
}

TEST_F(CsSliderBoxTest, getValuesAsString_DoubleValueCorrect)
{
    sliderBoxDouble->setMinMaxStepValue(0.0, 5.0, 0.01, 1.0);

    sliderBoxDouble->valueBoxDouble->setValue(3.5);

    EXPECT_EQ(sliderBoxDouble->getValuesAsString(), "3.5");
}

TEST_F(CsSliderBoxTest, getValuesAsString_IntValueCorrect)
{
    sliderBoxInt->setMinMaxStepValue(0, 300, 1, 112);

    sliderBoxInt->ui->slider->setValue(56);

    EXPECT_EQ(sliderBoxInt->getValuesAsString(), "56");
}

#endif // TST_CSSLIDERBOXTESTS_H
