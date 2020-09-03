#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QObject>
#include <QWidget>
#include <QLayout>

class PropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesView(QWidget *parent = nullptr);

private:
    QVBoxLayout* layout;

};

#endif // PROPERTIESVIEW_H
