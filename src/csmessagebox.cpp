#include "csmessagebox.h"

#include <QFontDatabase>

CsMessageBox::CsMessageBox(QWidget* parent)
    : QMessageBox(parent)
{
    this->setMinimumSize(500, 200);
    this->setTextFormat(Qt::MarkdownText);
}

CsMessageBox::~CsMessageBox()
{

}
