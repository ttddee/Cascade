#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QWidget>
#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT

public:
    ColorButton(QWidget *parent = nullptr);

    QColor getColor();
    void setColor(QColor);

private:
    void paintEvent(QPaintEvent*);

    QColor color;

signals:
    void colorChanged(QColor);

private slots:
    void handleButtonClicked();
};

#endif // COLORBUTTON_H
