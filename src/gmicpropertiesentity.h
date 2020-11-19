#ifndef GMICPROPERTIESENTITY_H
#define GMICPROPERTIESENTITY_H

#include <QWidget>

#include "uientity.h"
#include "gmichelper.h"

class NodeProperties;

namespace Ui {
class GmicPropertiesEntity;
}

class GmicPropertiesEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit GmicPropertiesEntity(
            UIElementType et,
            const QString& gType,
            const QString& gHash,
            QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~GmicPropertiesEntity();

private:
    Ui::GmicPropertiesEntity *ui;
    GmicHelper* gmicHelper;

    QString gmicNodeType;
    QString gmicHash;

    std::vector<UiEntity*> propElements;

signals:
    void valueChanged();
};

#endif // GMICPROPERTIESENTITY_H
