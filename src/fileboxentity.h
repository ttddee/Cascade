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
    void addEntries(const QStringList& entries);
    void deleteCurrentEntry();

    Ui::FileBoxEntity *ui;
    QStringListModel* fileListModel;

signals:
    void valueChanged();

private slots:
    void handleLoadButtonClicked();
    void handleDeleteButtonClicked();
};

#endif // FILEBOXENTITY_H
