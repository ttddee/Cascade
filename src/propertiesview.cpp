#include "propertiesview.h"

PropertiesView::PropertiesView(QWidget *parent)
    : QWidget(parent)
{
    layout = new QVBoxLayout();
    this->setLayout(layout);
}
