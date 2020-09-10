#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "nodeproperties.h"

class PropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesView(QWidget *parent = nullptr);

    void loadProperties(NodeProperties* prop);

private:
    QVBoxLayout* layout;
    NodeProperties* currentProperties = nullptr;

};

#endif // PROPERTIESVIEW_H
