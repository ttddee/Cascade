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

#ifndef NODEIN_H
#define NODEIN_H

#include <QObject>
#include <QPushButton>

#include "nodebase.h"
#include "connection.h"

class NodeInput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeInput(NodeInputType t, QWidget *parent = nullptr);

    void addInConnection(Connection*);
    void addInConnectionNoUpdate(Connection*);
    void removeInConnection();
    void updateConnection();
    bool hasConnection();

    void mousePressEvent(QMouseEvent*) override;

    Connection* getConnection();

    NodeInputType getInputType();

    QString getID() const;
    void setID(const QString& uuid);

    NodeBase* getUpstreamNode();

    NodeBase* parentNode = nullptr;

    Connection* inConnection = nullptr;

private:
    const NodeInputType inputType;

    QString id;


signals:
    void connectedNodeInputClicked(Connection* c);
};

#endif // NODEIN_H
