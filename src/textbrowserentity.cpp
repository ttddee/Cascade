#include "textbrowserentity.h"
#include "ui_textbrowserentity.h"

TextBrowserEntity::TextBrowserEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::TextBrowserEntity)
{
    ui->setupUi(this);

    ui->textBrowser->setReadOnly(true);
}

void TextBrowserEntity::setText(const QString &s)
{
    ui->textBrowser->setText(s);
}

QString TextBrowserEntity::getValuesAsString()
{
    return "";
}

TextBrowserEntity::~TextBrowserEntity()
{
    delete ui;
}
