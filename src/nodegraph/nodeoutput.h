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

#ifndef NODEOUT_H
#define NODEOUT_H

#include <QObject>
#include <QPushButton>
#include <QGraphicsItem>

#include "nodebase.h"
#include "connection.h"

namespace Cascade {

class NodeOutput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeOutput(QWidget *parent = nullptr);

    void addConnection(Connection* c);
    void removeConnection(Connection* c);
    void updateConnections();
    std::vector<Connection*> getConnections();

    NodeBase* mParentNode = nullptr;
    const int mVisualWidth = 10;
    const int mVisualHeight = 18;

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<Connection*> mOutConnections;

signals:
    void nodeOutputLeftMouseClicked(Cascade::NodeOutput* nodeOuptput);

};

} // namespace Cascade

#endif // NODEOUT_H
