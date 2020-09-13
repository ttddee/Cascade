#include "propertiesheading.h"
#include "ui_propertiesheading.h"

PropertiesHeading::PropertiesHeading(const QString& text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertiesHeading)
{
    ui->setupUi(this);

    ui->propHeading->setText(text);
}

PropertiesHeading::~PropertiesHeading()
{
    delete ui;
}
