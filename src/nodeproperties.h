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

#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "nodedefinitions.h"

using namespace Cascade;

class NodeBase;
class UiEntity;

class NodeProperties : public QWidget
{
    Q_OBJECT

public:
    explicit NodeProperties(
            const NodeType t,
            NodeBase* parentNode,
            QWidget *parent = nullptr);

    std::vector<UiEntity*> widgets;

private:
    const NodeType nodeType;
    QVBoxLayout* layout;
    NodeBase* parentNode;

signals:
    void projectIsDirty();

public slots:
    void handleSomeValueChanged();
    void handleFileSaveRequest(const QString& path);
};

#endif // NODEPROPERTIES_H
