#include "comboboxentity.h"
#include "ui_comboboxentity.h"

#include "nodeproperties.h"

ComboBoxEntity::ComboBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::ComboBoxEntity)
{
    ui->setupUi(this);

    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]{this->valueChanged();});
}

void ComboBoxEntity::setName(const QString &name)
{
    ui->label->setText(name);
}

void ComboBoxEntity::setOptions(const std::vector<QString> options, int index)
{
    foreach(auto& o, options)
    {
        ui->comboBox->addItem(o);
    }
    ui->comboBox->setCurrentIndex(index);
}

void ComboBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ComboBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString ComboBoxEntity::getValuesAsString()
{
    return QString::number(ui->comboBox->currentIndex());
}

ComboBoxEntity::~ComboBoxEntity()
{
    delete ui;
}
