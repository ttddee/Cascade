#ifndef SPINBOXENTITY_H
#define SPINBOXENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

using namespace Cascade;

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

#endif // SPINBOXENTITY_H
