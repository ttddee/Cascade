#ifndef WRITEPROPERTIESENTITY_H
#define WRITEPROPERTIESENTITY_H

#include <QWidget>

#include "uientity.h"
#include "nodeproperties.h"

namespace Ui {
class WritePropertiesEntity;
}

class WritePropertiesEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit WritePropertiesEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToRequestFileSave(NodeProperties* p);
    QString getValuesAsString() override;

    ~WritePropertiesEntity();

private:
    void setFileName(const QString& f);
    void setFolder(const QString& f);
    void updateFileNameLabel();

    Ui::WritePropertiesEntity *ui;

    QString fileName = "unnamed";
    QString folder;

signals:
    void requestFileSave(const QString& path);

public slots:
    void handleFileNametextChanged();
    void handleSetFolderButtonClicked();
    void handleSaveFileButtonClicked();
};

#endif // WRITEPROPERTIESENTITY_H
