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

    mValueBox = new QDoubleSpinBox(this);
    mValueBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    mLayout->addWidget(mValueBox, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);
    mValueBox->installEventFilter(this);

    if (sliderType == SliderType::Int)
    {
        mValueBox->setDecimals(0);
    }

    mSlider->installEventFilter(this);
    mNameLabel->installEventFilter(this);

    connect(mSlider, &QSlider::valueChanged,
            this, &Slider::handleSliderValueChanged);

    connect(mValueBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &Slider::handleSpinBoxValueChanged);
}

void Slider::setMinMaxStepValue(double min, double max, double step, double value)
{
    mBaseValue = value;

    mValueBox->blockSignals(true);
    mValueBox->setMinimum(min);
    mValueBox->setMaximum(max);
    mValueBox->setSingleStep(step);
    mValueBox->setValue(value);
    mValueBox->blockSignals(false);

    if(mSliderType == SliderType::Double)
    {
        min *= doubleMult;
        max *= doubleMult;
        step *= doubleMult;
        value *= doubleMult;
    }

    mSlider->blockSignals(true);
    mSlider->setMinimum(min);
    mSlider->setMaximum(max);
    mSlider->setSingleStep(step);
    mSlider->setValue(value);
    mSlider->blockSignals(false);
}

void Slider::setName(const QString& name)
{
    mNameLabel->setText(name);
}

void Slider::setSliderNoSignal(double val)
{
    if (mSliderType == SliderType::Double)
    {
        val *= doubleMult;
    }
    mSlider->blockSignals(true);
    mSlider->setValue(val);
    mSlider->blockSignals(false);

    emit valueChanged();
}

void Slider::setSpinBoxNoSignal(double val)
{
    mValueBox->blockSignals(true);

    if (mSliderType == SliderType::Double)
    {
        val /= doubleMult;
    }
    mValueBox->setValue(val);
    mValueBox->blockSignals(false);

    emit valueChanged();
}

double Slider::getValue() const
{
    return mValueBox->value();
}

void Slider::handleSliderValueChanged()
{
    setSpinBoxNoSignal(mSlider->value());
}

void Slider::handleSpinBoxValueChanged()
{
    setSliderNoSignal(mValueBox->value());
}

void Slider::reset()
{
    mValueBox->setValue(mBaseValue);
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
