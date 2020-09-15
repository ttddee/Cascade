#include "spinboxsliderentity.h"
#include "ui_spinboxsliderentity.h"

#include <QMouseEvent>
#include <QSlider>

SpinBoxSliderEntity::SpinBoxSliderEntity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpinBoxSliderEntity)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setVisible(false);

    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this]{this->setSliderNoSignal(ui->spinBox->value());});
    connect(ui->doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this]{this->setSliderNoSignal(ui->doubleSpinBox->value());});
    connect(ui->slider, &QSlider::valueChanged,
            this, [this]{this->setSpinBoxNoSignal(ui->slider->value());});
    connect(ui->resetButton, &QPushButton::clicked,
            this, &SpinBoxSliderEntity::reset);
    // Only emit valueChanged on sliderRelease and Spinbox change
    connect(ui->slider, &QSlider::sliderReleased,
            this, &SpinBoxSliderEntity::handleSliderReleased);
    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SpinBoxSliderEntity::handleSliderReleased);
    connect(ui->doubleSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &SpinBoxSliderEntity::handleSliderReleased);
}

SpinBoxSliderEntity::~SpinBoxSliderEntity()
{
    delete ui;
}

void SpinBoxSliderEntity::makeDouble()
{
    ui->spinBox->setVisible(false);
    ui->doubleSpinBox->setVisible(true);
    isDouble = true;
}

void SpinBoxSliderEntity::setName(const QString &s)
{
    ui->label->setText(s);
}

void SpinBoxSliderEntity::setMinMaxStepValue(int min, int max, int step, int value)
{
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
    ui->spinBox->setSingleStep(step);
    ui->spinBox->setValue(value);
    ui->slider->setMinimum(min);
    ui->slider->setMaximum(max);
    ui->slider->setSingleStep(step);
    ui->slider->setValue(value);
    baseValue = value;
}

void SpinBoxSliderEntity::setMinMaxStepValue(double min, double max, double step, double value)
{
    ui->doubleSpinBox->setMinimum(min);
    ui->doubleSpinBox->setMaximum(max);
    ui->doubleSpinBox->setSingleStep(step);
    ui->doubleSpinBox->setValue(value);
    ui->slider->setMinimum(static_cast<int>(min * 100));
    ui->slider->setMaximum(static_cast<int>(max * 100));
    ui->slider->setSingleStep(static_cast<int>(step * 100));
    ui->slider->setValue(static_cast<int>(value * 100));
    baseValue = static_cast<int>(value * 100);
}

void SpinBoxSliderEntity::setSpinBoxNoSignal(int i)
{
    if (isDouble)
    {
        ui->doubleSpinBox->blockSignals(true);
        ui->doubleSpinBox->setValue(i / 100.0);
        ui->doubleSpinBox->blockSignals(false);
        setCurrentValue(i / 100.0);
    }
    else
    {
        ui->spinBox->blockSignals(true);
        ui->spinBox->setValue(i);
        ui->spinBox->blockSignals(false);
        setCurrentValue(i);
    }
}

void SpinBoxSliderEntity::setSliderNoSignal(int i)
{
    ui->slider->blockSignals(true);
    ui->slider->setValue(i);
    ui->slider->blockSignals(false);
    setCurrentValue(i);
}

void SpinBoxSliderEntity::setSliderNoSignal(double d)
{
    ui->slider->blockSignals(true);
    ui->slider->setValue(static_cast<int>(d * 100));
    ui->slider->blockSignals(false);
    setCurrentValue(d);
}

void SpinBoxSliderEntity::setCurrentValue(int i)
{
    currentValue = i;
    //emit valueChangedInt(i);
}

void SpinBoxSliderEntity::setCurrentValue(double d)
{
    currentValue = static_cast<int>(d * 100);
    //emit valueChangedDouble(d);
}

void SpinBoxSliderEntity::reset()
{
    ui->spinBox->setValue(baseValue);
    ui->doubleSpinBox->setValue(baseValue / 100);

    emit valueChangedInt(currentValue);
    emit valueChangedDouble((double)currentValue / 100);
}

void SpinBoxSliderEntity::selfConnectToValueChanged(NodeProperties* p)
{
    if (!isDouble)
    {
        connect(this, &SpinBoxSliderEntity::valueChangedInt,
                p, [p]{p->handleSomeValueChanged();});
    }
    else
    {
        connect(this, &SpinBoxSliderEntity::valueChangedDouble,
                p, [p]{p->handleSomeValueChanged();});
    }
}

QString SpinBoxSliderEntity::returnValueAsString()
{
    if (isDouble)
        return QString::number(currentValue / 100.0);
    else
        return QString::number(currentValue);
}

void SpinBoxSliderEntity::handleSliderReleased()
{
    emit valueChangedInt(currentValue);
    emit valueChangedDouble((double)currentValue / 100);
}


