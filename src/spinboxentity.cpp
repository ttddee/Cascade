#include "spinboxentity.h"
#include "ui_spinboxentity.h"

#include "nodeproperties.h"

SpinBoxEntity::SpinBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::SpinBoxEntity)
{
    ui->setupUi(this);

    connect(ui->spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [this]{this->valueChanged();});
    connect(ui->resetButton, &QPushButton::clicked,
            this, &SpinBoxEntity::reset);
}

void SpinBoxEntity::setName(const QString &name)
{
    ui->nameLabel->setText(name);
}

void SpinBoxEntity::setMinMaxStepValue(int min, int max, int step, int value)
{
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
    ui->spinBox->setSingleStep(step);
    ui->spinBox->setValue(value);
    baseValue = value;
}

void SpinBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &SpinBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

void SpinBoxEntity::reset()
{
    ui->spinBox->setValue(baseValue);
    emit valueChanged();
}

QString SpinBoxEntity::getValuesAsString()
{
    return QString::number(ui->spinBox->value());
}

SpinBoxEntity::~SpinBoxEntity()
{
    delete ui;
}
