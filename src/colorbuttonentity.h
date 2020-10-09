#ifndef COLORBUTTONENTITY_H
#define COLORBUTTONENTITY_H

#include <QWidget>

#include "uientity.h"
#include "nodeproperties.h"

namespace Ui {
class ColorButtonEntity;
}

class ColorButtonEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit ColorButtonEntity(UIElementType et, QWidget *parent = nullptr);

    void setName(const QString&);
    QColor getColor();
    void setColor(QColor);
    void selfConnectToValueChanged(NodeProperties*);

    QString getValuesAsString();

    ~ColorButtonEntity();

private:
    Ui::ColorButtonEntity *ui;

private slots:
    void handleColorChanged(QColor);

signals:
    void valueChanged(QColor);
};

#endif // COLORBUTTONENTITY_H
