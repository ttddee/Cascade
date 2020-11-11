#ifndef SLIDERNOCLICK_H
#define SLIDERNOCLICK_H

#include <QSlider>
#include <QObject>

class SliderNoClick : public QSlider
{
    Q_OBJECT

public:
    SliderNoClick(QWidget *parent = nullptr);

private:
    void mousePressEvent(QMouseEvent*) override;
};

#endif // SLIDERNOCLICK_H
