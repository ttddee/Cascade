#include "checkboxentity.h"
#include "ui_checkboxentity.h"

#include "nodeproperties.h"

CheckBoxEntity::CheckBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::CheckBoxEntity)
{
    ui->setupUi(this);

    connect(ui->checkBox, &QCheckBox::stateChanged,
            this, &CheckBoxEntity::valueChanged);
}

void CheckBoxEntity::setName(const QString &name)
{
    ui->checkBox->setText(name);
}

void CheckBoxEntity::setChecked(bool b)
{
    ui->checkBox->setChecked(b);
}

void CheckBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &CheckBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString CheckBoxEntity::getValuesAsString()
{
    return QString::number(ui->checkBox->isChecked());
}

CheckBoxEntity::~CheckBoxEntity()
{
    delete ui;
}
