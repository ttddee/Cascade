#include "colorbuttonentity.h"
#include "ui_colorbuttonentity.h"

#include "nodeproperties.h"

ColorButtonEntity::ColorButtonEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::ColorButtonEntity)
{
    ui->setupUi(this);

    connect(ui->colorButton, &ColorButton::colorChanged,
                this, &ColorButtonEntity::handleColorChanged);
}

QColor ColorButtonEntity::getColor()
{
    return ui->colorButton->getColor();
}

void ColorButtonEntity::setColor(QColor c)
{
    ui->colorButton->setColor(c);
}

void ColorButtonEntity::setName(const QString &s)
{
    ui->label->setText(s);
}

void ColorButtonEntity::handleColorChanged(QColor c)
{
    emit valueChanged(c);
}

QString ColorButtonEntity::getValuesAsString()
{
    QString s;
    s.append(QString::number(getColor().red()));
    s.append(",");
    s.append(QString::number(getColor().green()));
    s.append(",");
    s.append(QString::number(getColor().blue()));
    // Omitting alpha

    return s;
}

void ColorButtonEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &ColorButtonEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

ColorButtonEntity::~ColorButtonEntity()
{
    delete ui;
}
