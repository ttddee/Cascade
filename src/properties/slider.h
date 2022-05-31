#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>
#include <QMouseEvent>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QGridLayout>

namespace Cascade::Properties {

constexpr float doubleMult = 100.0;

enum class SliderType
{
    Int,
    Double
};

class Slider : public QWidget
{
    Q_OBJECT

public:
    explicit Slider(
        SliderType sliderType,
        QWidget *parent = nullptr);

    template<typename T>
    void setMinMaxStepValue(
        T min,
        T max,
        T step,
        T value)
    {
        mBaseValue = value;

        if (std::is_same<T , double>::value)
        {
            mValueBoxDouble->blockSignals(true);
            mValueBoxDouble->setMinimum(min);
            mValueBoxDouble->setMaximum(max);
            mValueBoxDouble->setSingleStep(step);
            mValueBoxDouble->setValue(value);
            mValueBoxDouble->blockSignals(false);

            mSlider->blockSignals(true);
            mSlider->setMinimum(min *= doubleMult);
            mSlider->setMaximum(max *= doubleMult);
            mSlider->setSingleStep(step *= doubleMult);
            mSlider->setValue(value *= doubleMult);
            mSlider->blockSignals(false);
        }
        else
        {
            mValueBoxInt->blockSignals(true);
            mValueBoxInt->setMinimum(min);
            mValueBoxInt->setMaximum(max);
            mValueBoxInt->setSingleStep(step);
            mValueBoxInt->setValue(value);
            mValueBoxInt->blockSignals(false);

            mSlider->blockSignals(true);
            mSlider->setMinimum(min);
            mSlider->setMaximum(max);
            mSlider->setSingleStep(step);
            mSlider->setValue(value);
            mSlider->blockSignals(false);
        }
    }

    void setName(const QString& name);

private:
    template<typename T>
    void setSliderNoSignal(T val)
    {
        if (std::is_same<T , double>::value)
        {
            val *= doubleMult;
        }
        mSlider->blockSignals(true);
        mSlider->setValue(static_cast<int>(val));
        mSlider->blockSignals(false);

        emit valueChanged();
    }

    template<typename T>
    void setSpinBoxNoSignal(T val)
    {
        if (mSliderType == SliderType::Double)
        {
            mValueBoxDouble->blockSignals(true);
            mValueBoxDouble->setValue(val / doubleMult);
            mValueBoxDouble->blockSignals(false);
        }
        else
        {
            mValueBoxInt->blockSignals(true);
            mValueBoxInt->setValue(val);
            mValueBoxInt->blockSignals(false);
        }

        emit valueChanged();
    }

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;

    QLabel* mNameLabel;
    QDoubleSpinBox* mValueBoxDouble;
    QSpinBox* mValueBoxInt;
    QSlider* mSlider;
    QGridLayout* mLayout;

    bool mIsDragging = false;
    float mBaseValue;
    QPointF mLastPos;

    const SliderType mSliderType;

signals:
    void valueChanged();

private slots:
    void handleSliderValueChanged();
    void handleSpinBoxValueChanged();
};

} // namespace Cascade::Properties

#endif // SLIDER_H
