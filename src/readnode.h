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

#ifndef READNODE_H
#define READNODE_H

#include "nodebase.h"

namespace Cascade {

class ReadNode : public NodeBase
{
    Q_OBJECT

public:
    ReadNode(const NodeType type,
             NodeGraph* graph,
             QWidget *parent = nullptr,
             const QString& customName = "");

    bool canBeRendered() const override;
};

} // namespace Cascade

#endif // READNODE_H