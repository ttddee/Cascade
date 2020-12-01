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

#ifndef GMICPROPERTIESENTITY_H
#define GMICPROPERTIESENTITY_H

#include <set>

#include <QWidget>

#include "uientity.h"
#include "../gmichelper.h"
#include "checkboxentity.h"

class NodeProperties;

namespace Ui {
class GmicPropertiesEntity;
}

class GmicPropertiesEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit GmicPropertiesEntity(
            UIElementType et,
            const QString& gType,
            const QString& gHash,
            QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);
    QString getValuesAsString() override;

    ~GmicPropertiesEntity();

private:
    Ui::GmicPropertiesEntity *ui;
    GmicHelper* gmicHelper;

    QString gmicNodeType;
    QString gmicHash;

    bool hasPreviewSplitProperty = false;

    CheckBoxEntity* previewBox;

    std::vector<UiEntity*> propElements;
    const std::set<QString> hiddenElements =
    {
        "Preview Type",
        "Output Each Piece on a Different Layer"
    };

signals:
    void valueChanged();
};

#endif // GMICPROPERTIESENTITY_H
