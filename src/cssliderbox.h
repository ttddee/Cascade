#ifndef CSSLIDERBOX_H
#define CSSLIDERBOX_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class CsSliderBox;
}

class CsSliderBox : public UiEntity
{
    Q_OBJECT

public:
    explicit CsSliderBox(UIElementType et, QWidget *parent = nullptr);

    void setMinMaxStepValue(double, double, double, double);
    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~CsSliderBox();

private:
    void setSpinBoxNoSignal(int i);
    void setSliderNoSignal(double d);

    bool eventFilter(QObject* watched, QEvent* event) override;

    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;

    Ui::CsSliderBox *ui;

    QLabel* nameLabel;
    QDoubleSpinBox* valueBox;

    bool isDragging = false;
    bool controlPressed = false;
    QPoint lastPos;

    int baseValue;

signals:
    void valueChanged(double d);

};

#endif // CSSLIDERBOX_H
