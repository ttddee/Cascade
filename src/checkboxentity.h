#ifndef CHECKBOXENTITY_H
#define CHECKBOXENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class CheckBoxEntity;
}

class CheckBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit CheckBoxEntity(UIElementType et, QWidget *parent = nullptr);

    void setName(const QString& name);

    void setChecked(bool b);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~CheckBoxEntity();

private:
    Ui::CheckBoxEntity *ui;

signals:
    void valueChanged();
};

#endif // CHECKBOXENTITY_H
