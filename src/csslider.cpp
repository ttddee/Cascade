#include "csslider.h"

#include <QMouseEvent>

CsSlider::CsSlider(QWidget* parent)
    : QSlider(parent)
{

}

void CsSlider::mouseReleaseEvent(QMouseEvent * event)
{
    // Enable update on slider click
    if (event->button() == Qt::LeftButton)
        emit sliderReleased();
    QSlider::mouseReleaseEvent(event);
}

void CsSlider::wheelEvent(QWheelEvent * e)
{
    // No mouse wheel scrolling

    Q_UNUSED(e);
}
