#ifndef FILEBOXENTITY_H
#define FILEBOXENTITY_H

#include <QWidget>
#include <QStringListModel>

#include "nodedefinitions.h"
#include "uientity.h"

class NodeProperties;

using namespace Cascade;

namespace Ui {
class FileBoxEntity;
}

class FileBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit FileBoxEntity(UIElementType et, QWidget *parent = nullptr);

    QString getCurrentPath();
    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~FileBoxEntity();

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
