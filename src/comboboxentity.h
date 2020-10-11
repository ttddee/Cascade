#ifndef COMBOBOXENTITY_H
#define COMBOBOXENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

using namespace Cascade;

namespace Ui {
class ComboBoxEntity;
}

class ComboBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit ComboBoxEntity(UIElementType et, QWidget *parent = nullptr);

    void setName(const QString& name);
    void setOptions(const std::vector<QString> options);
    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~ComboBoxEntity();

private:
    Ui::ComboBoxEntity *ui;

signals:
    void valueChanged();
};

#endif // COMBOBOXENTITY_H
