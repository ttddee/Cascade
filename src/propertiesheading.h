#ifndef PROPERTIESHEADING_H
#define PROPERTIESHEADING_H

#include <QWidget>

namespace Ui {
class PropertiesHeading;
}

class PropertiesHeading : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesHeading(const QString& text, QWidget *parent = nullptr);

    ~PropertiesHeading();

private:
    Ui::PropertiesHeading *ui;
};

#endif // PROPERTIESHEADING_H
