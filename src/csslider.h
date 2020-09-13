#ifndef CSSLIDER_H
#define CSSLIDER_H

#include <QWidget>
#include <QSlider>

class CsSlider : public QSlider
{
    Q_OBJECT
public:
    CsSlider(QWidget* parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
};

#endif // CSSLIDER_H
