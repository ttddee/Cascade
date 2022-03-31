/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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

#ifndef FOLDERBOXENTITY_H
#define FOLDERBOXENTITY_H

#include <QWidget>

#include "uientity.h"

namespace Ui {
class FolderBoxEntity;
}

namespace Cascade {

class NodeProperties;

class FolderBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit FolderBoxEntity(UIElementType et, QWidget *parent = nullptr);

    const QString name() override;
    void setName(const QString& name);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    ~FolderBoxEntity();

private:
    Ui::FolderBoxEntity *ui;

signals:
    void valueChanged();

public slots:
    void handleFolderButtonClicked();
};

} // namespace Cascade

#endif // FOLDERBOXENTITY_H
