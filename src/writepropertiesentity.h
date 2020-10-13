/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef WRITEPROPERTIESENTITY_H
#define WRITEPROPERTIESENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

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
