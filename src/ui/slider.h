#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>
#include <QMouseEvent>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QGridLayout>

#include "../log.h"

namespace Cascade::UiElements {

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

    void setMinMaxStepValue(
        double min,
        double max,
        double step,
        double value);

    double getValue() const;

    void setName(const QString& name);

private:
    void setSliderNoSignal(double val);

    void setSpinBoxNoSignal(double val);

    void reset();

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;

    QLabel*             mNameLabel;
    QDoubleSpinBox*     mValueBox;
    QSlider*            mSlider;
    QGridLayout*        mLayout;

    bool                mIsDragging = false;
    double              mBaseValue;
    QPointF             mLastPos;

    const SliderType    mSliderType;

signals:
    void valueChanged();

private slots:
    void handleSliderValueChanged();
    void handleSpinBoxValueChanged();
};

} // namespace Cascade::UiElements

#endif // SLIDER_H
