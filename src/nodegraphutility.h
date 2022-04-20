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

#ifndef NODEGRAPHUTILITY_H
#define NODEGRAPHUTILITY_H

#include "nodedefinitions.h"

namespace Cascade {

struct NodePersistentProperties
{
    NodeType mNodeType;
    QPoint mPos;
    QString mUuid;
    QMap<int, QString> mInputs;
    QMap<int, QString> mProperties;
    QString mCustomName;
};

enum class NodeGraphPosition
{
    eRelativeToLastNode,
    eAtCursor,
    eCustom
};

} // namespace Cascade

#endif // NODEGRAPHUTILITY_H
