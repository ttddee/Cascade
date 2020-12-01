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

#ifndef CHECKBOXENTITY_H
#define CHECKBOXENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class CheckBoxEntity;
}

class CheckBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit CheckBoxEntity(UIElementType et, QWidget *parent = nullptr);

    const QString name() override;
    void setName(const QString& name);

    void setChecked(bool b);
    bool isChecked();

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~CheckBoxEntity();

private:
    Ui::CheckBoxEntity *ui;

signals:
    void valueChanged();
};

#endif // CHECKBOXENTITY_H
