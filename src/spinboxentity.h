#ifndef COMBOBOXENTITY_H
#define COMBOBOXENTITY_H

#include <QWidget>

#include "nodeproperties.h"

namespace Ui {
class SpinBoxEntity;
}

class SpinBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SpinBoxEntity(UIElementType et, QWidget *parent = nullptr);

    void setName(const QString& name);
    void setMinMaxStepValue(int, int, int, int);
    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~SpinBoxEntity();

private:
    void reset();

    Ui::SpinBoxEntity *ui;

    int baseValue = 0;

signals:
    void valueChanged();
};

#endif // COMBOBOXENTITY_H
