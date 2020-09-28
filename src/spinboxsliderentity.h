#ifndef SPINBOXSLIDERENTITY_H
#define SPINBOXSLIDERENTITY_H

#include <QWidget>

#include "nodeproperties.h"

namespace Ui {
class SpinBoxSliderEntity;
}

class SpinBoxSliderEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SpinBoxSliderEntity(UIElementType et, QWidget *parent = nullptr);

    void makeDouble();
    void setName(const QString& name);
    void setMinMaxStepValue(int, int, int, int);
    void setMinMaxStepValue(double, double, double, double);
    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    auto value()
    {
        return isDouble ? currentValue / 100.0 : currentValue;
    }

    ~SpinBoxSliderEntity();

private:
    void setSpinBoxNoSignal(int);
    void setSliderNoSignal(int);
    void setSliderNoSignal(double);
    void setCurrentValue(int);
    void setCurrentValue(double);
    void reset();

    Ui::SpinBoxSliderEntity *ui;

    bool isDouble = false;
    int baseValue = 0;
    int currentValue = 0;

signals:
    void valueChangedInt(int);
    void valueChangedDouble(double);

private slots:
    void handleSliderReleased();

};

#endif // SPINBOXSLIDERENTITY_H
