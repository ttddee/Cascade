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

#ifndef NODEGRAPHDATA_H
#define NODEGRAPHDATA_H

#include <memory>

#include <QObject>

#include "connection.h"
#include "node.h"

namespace Cascade::NodeGraph
{

class NodeGraphData : public QObject
{
    Q_OBJECT

public:
    explicit NodeGraphData(QObject *parent = nullptr);

    std::unordered_map<QUuid, std::unique_ptr<Node>> const& nodes() const;

    std::unordered_map<QUuid, std::shared_ptr<Connection>> const& connections() const;

private:
    using SharedConnection = std::shared_ptr<Connection>;
    using UniqueNode       = std::unique_ptr<Node>;

    std::unordered_map<QUuid, SharedConnection> mConnections;
    std::unordered_map<QUuid, UniqueNode>       mNodes;

};

} // namespace Cascade::NodeGraph

#endif // NODEGRAPHDATA_H
