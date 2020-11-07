#ifndef CHANNELSELECTENTITY_H
#define CHANNELSELECTENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class ChannelSelectEntity;
}

class ChannelSelectEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit ChannelSelectEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~ChannelSelectEntity();

private:
    Ui::ChannelSelectEntity *ui;

signals:
    void valueChanged();
};

#endif // CHANNELSELECTENTITY_H
