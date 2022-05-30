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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#pragma once

#include <vector>
#include <unordered_map>

#include <QtCore/QUuid>

#include "porttype.h"
#include "nodedata.h"
#include "memory.h"

namespace Cascade::NodeGraph
{

class Connection;
class NodeDataModel;

/// Contains vectors of connected input and output connections.
/// Stores bool for reacting on hovering connections
class NodeState
{
public:
    enum ReactToConnectionState
    {
        REACTING,
        NOT_REACTING
    };

public:
    NodeState(std::unique_ptr<NodeDataModel> const &model);

public:
    using ConnectionPtrSet = std::unordered_map<QUuid, Connection*>;

    /// Returns vector of connection IDs.
    /// Some of them can be empty (null)
    std::vector<ConnectionPtrSet> const& getEntries(PortType) const;

    std::vector<ConnectionPtrSet>& getEntries(PortType);

    ConnectionPtrSet connections(PortType portType, PortIndex portIndex) const;

    void setConnection(
        PortType portType,
        PortIndex portIndex,
        Connection& connection);

    void eraseConnection(
        PortType portType,
        PortIndex portIndex,
        QUuid id);

    ReactToConnectionState reaction() const;

    PortType reactingPortType() const;

    NodeDataType reactingDataType() const;

    void setReaction(
        ReactToConnectionState reaction,
        PortType reactingPortType = PortType::None,
        NodeDataType reactingDataType = NodeDataType());

    bool isReacting() const;

    void setResizing(bool resizing);

    bool resizing() const;

private:
    std::vector<ConnectionPtrSet> mInConnections;
    std::vector<ConnectionPtrSet> mOutConnections;

    ReactToConnectionState mReaction;
    PortType     mReactingPortType;
    NodeDataType mReactingDataType;

    bool mResizing;
};
}
