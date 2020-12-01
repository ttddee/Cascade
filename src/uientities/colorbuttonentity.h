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

#ifndef COLORBUTTONENTITY_H
#define COLORBUTTONENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

namespace Ui {
class ColorButtonEntity;
}

class ColorButtonEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit ColorButtonEntity(UIElementType et, QWidget *parent = nullptr);

    const QString name() override;
    void setName(const QString&);

    QColor getColor();
    void setColor(QColor);

    void selfConnectToValueChanged(NodeProperties*);

    QString getValuesAsString() override;

    ~ColorButtonEntity();

private:
    Ui::ColorButtonEntity *ui;

private slots:
    void handleColorChanged(QColor);

signals:
    void valueChanged(QColor);
};

#endif // COLORBUTTONENTITY_H
