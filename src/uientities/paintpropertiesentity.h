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

#ifndef PAINTPROPERTIESENTITY_H
#define PAINTPROPERTIESENTITY_H

#include <QWidget>

#include "uientity.h"

class NodeProperties;

class PaintPropertiesEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit PaintPropertiesEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties*);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

private:
    QPoint viewerClickPos;

public slots:
    void handleViewerClick(QPoint pos);

signals:
    void valueChanged();
};

#endif // PAINTPROPERTIESENTITY_H
