#include "channelselectentity.h"
#include "ui_channelselectentity.h"

#include <iostream>

#include "nodeproperties.h"

ChannelSelectEntity::ChannelSelectEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    ui(new Ui::ChannelSelectEntity)
{
    ui->setupUi(this);

    connect(ui->redBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(ui->greenBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(ui->blueBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
    connect(ui->alphaBox, &QCheckBox::toggled,
            this, &ChannelSelectEntity::valueChanged);
}

void ChannelSelectEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ChannelSelectEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString ChannelSelectEntity::getValuesAsString()
{
    auto s = QString::number(ui->redBox->isChecked()) + "," +
             QString::number(ui->greenBox->isChecked()) + "," +
             QString::number(ui->blueBox->isChecked()) + "," +
             QString::number(ui->alphaBox->isChecked());
    return s;
}

ChannelSelectEntity::~ChannelSelectEntity()
{
    delete ui;
}
