#ifndef SPINBOXSLIDERENTITY_H
#define SPINBOXSLIDERENTITY_H

#include <QWidget>

namespace Ui {
class SpinBoxSliderEntity;
}

class SpinBoxSliderEntity : public QWidget
{
    Q_OBJECT

public:
    explicit SpinBoxSliderEntity(QWidget *parent = nullptr);
    ~SpinBoxSliderEntity();
    void makeDouble();
    void setName(const std::string&);
    void setMinMaxStepValue(int, int, int, int);
    void setMinMaxStepValue(double, double, double, double);

    //void selfConnectToValueChanged(NodePropertiesBase*); // needs template
    QString returnValueAsString();

    auto value()
    {
        return isDouble ? currentValue / 100.0 : currentValue;
    }

private:
    Ui::SpinBoxSliderEntity *ui;
    void setSpinBoxNoSignal(int);
    void setSliderNoSignal(int);
    void setSliderNoSignal(double);
    void setCurrentValue(int);
    void setCurrentValue(double);
    void reset();

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
