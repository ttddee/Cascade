#include "slidernoclick.h"

SliderNoClick::SliderNoClick(QWidget *parent)
    : QSlider(parent)
{

}

void
 SliderNoClick::mousePressEvent(QMouseEvent* event)
{
    // No click
    Q_UNUSED(event);
}
