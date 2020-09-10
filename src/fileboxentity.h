#ifndef FILEBOXENTITY_H
#define FILEBOXENTITY_H

#include <QWidget>
#include <QStringListModel>

#include "nodedefinitions.h"
#include "uientity.h"

using namespace Cascade;

namespace Ui {
class FileBoxEntity;
}

class FileBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit FileBoxEntity(UIElementType et, QWidget *parent = nullptr);
    ~FileBoxEntity();
    QString getCurrentPath();

private:
    void setCurrentState(const QString&);

    Ui::FileBoxEntity *ui;
    QStringListModel* fileListModel;

signals:
    void valueChanged();

private slots:
    void handleLoadButtonClicked();
    void handleDeleteButtonClicked();
};

#endif // FILEBOXENTITY_H
