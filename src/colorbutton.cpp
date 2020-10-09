#include "colorbutton.h"

#include <iostream>

#include <QColorDialog>
#include <QPainter>
#include <QPaintEvent>

ColorButton::ColorButton(QWidget *parent) :
    QPushButton(parent)
{
    this->setText("");

    connect(this, &ColorButton::clicked,
            this, &ColorButton::handleButtonClicked);

    color = Qt::white;
}

QColor ColorButton::getColor()
{
    return color;
}

void ColorButton::setColor(QColor c)
{
    color = c;

    emit colorChanged(c);
}

void ColorButton::handleButtonClicked()
{
    QColor c = QColorDialog::getColor(this->getColor());

    if (c.isValid())
        setColor(c);
}

void ColorButton::paintEvent(QPaintEvent *e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);

    QRect r(3, 3, 24, 14);
    painter.setBrush(this->getColor());
    painter.setPen(Qt::NoPen);
    painter.drawRect(r);
}
