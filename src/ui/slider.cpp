#include "slider.h"

#include <QApplication>

namespace Cascade::UiElements {

Slider::Slider(
    SliderType sliderType,
    QWidget *parent) :
    QWidget(parent),
    mSliderType(sliderType)
{
    mLayout = new QGridLayout();
    mLayout->setVerticalSpacing(0);
    mLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mLayout);

    mSlider = new QSlider(Qt::Horizontal, this);
    mSlider->setMinimumHeight(22);
    mLayout->addWidget(mSlider);

    mNameLabel = new QLabel(this);
    mNameLabel->setText("None");
    mNameLabel->setObjectName("nameLabel");
    mLayout->addWidget(mNameLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    if (sliderType == SliderType::Double)
    {
        mValueBoxDouble = new QDoubleSpinBox(this);
        mValueBoxDouble->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mLayout->addWidget(mValueBoxDouble, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        mValueBoxDouble->installEventFilter(this);
    }
    else
    {
        mValueBoxInt = new QSpinBox(this);
        mValueBoxInt->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mLayout->addWidget(mValueBoxInt, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        mValueBoxInt->installEventFilter(this);
    }

    mSlider->installEventFilter(this);
    mNameLabel->installEventFilter(this);

    connect(mSlider, &QSlider::valueChanged,
            this, &Slider::handleSliderValueChanged);

    if (mSliderType == SliderType::Double)
        connect(mValueBoxDouble, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &Slider::handleSpinBoxValueChanged);
    else
        connect(mValueBoxInt, QOverload<int>::of(&QSpinBox::valueChanged),
                this, &Slider::handleSpinBoxValueChanged);
}

void Slider::setName(const QString& name)
{
    mNameLabel->setText(name);
}

void Slider::handleSliderValueChanged()
{
    setSpinBoxNoSignal(mSlider->value());
}

void Slider::handleSpinBoxValueChanged()
{
    if (mSliderType == SliderType::Double)
        setSliderNoSignal(mValueBoxDouble->value());
    else
        setSliderNoSignal(mValueBoxInt->value());
}

void Slider::reset()
{
    if (mSliderType == SliderType::Double)
        mValueBoxDouble->setValue(mBaseValue);
    else
        mValueBoxInt->setValue(mBaseValue);
}

bool Slider::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            mIsDragging = true;
            // factor for scaling pixels into value of slider
            double factorPixelsToValue = mSlider->maximum() / static_cast<double>(this->size().width());
            mSlider->setValue( static_cast<int>(mouseEvent->x() *factorPixelsToValue));

            if (QApplication::queryKeyboardModifiers() == Qt::ControlModifier)
            {
                reset();
            }
        }
    }
    if(event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if(mouseEvent->button() == Qt::LeftButton)
        {
            mIsDragging = false;
        }
    }

    Q_UNUSED(watched);

    return false;
}

void Slider::mouseMoveEvent(QMouseEvent* event)
{
    // lambda function to get sign of the value
    auto sign = [](int a) { return (0 < a) - (a < 0); };

    if(mIsDragging)
    {
        // factor for scaling pixels into value of slider
        double factorPixelsToValue = mSlider->maximum() / static_cast<double>(this->size().width());
        auto cursorPos = event->windowPos();
        int sliderValue = mSlider->value();
        auto offsetFromPrev = (cursorPos.x() - mLastPos.x());
        auto newPos = event->x() * factorPixelsToValue;

        // when shift is pressed we want to have more precision in the slider movement
        if (QApplication::queryKeyboardModifiers() == Qt::ShiftModifier)
            newPos = sliderValue + sign(offsetFromPrev) * mSlider->singleStep();

        mSlider->setValue(newPos);
        mLastPos = cursorPos;
    }
    Q_UNUSED(event);
}

} // namespace Cascade::UiElements
