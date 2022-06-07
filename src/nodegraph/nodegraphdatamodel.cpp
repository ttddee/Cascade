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

#include "nodegraphdatamodel.h"

namespace Cascade::NodeGraph
{

NodeGraphDataModel::NodeGraphDataModel(
    NodeGraphScene* scene,
    QObject *parent)
    : QObject(parent)
{
    mScene = scene;
}

NodeGraphData* NodeGraphDataModel::data() const
{
    return mData.get();
}

Node& NodeGraphDataModel::createNode(std::unique_ptr<NodeDataModel>&& dataModel)
{
    auto node = std::make_unique<Node>(std::move(dataModel));
    auto ngo  = std::make_unique<NodeGraphicsObject>(*mScene, *node);

    node->setGraphicsObject(std::move(ngo));

    auto nodePtr = node.get();
    mData->mNodes[node->id()] = std::move(node);

    //emit nodeCreated(*nodePtr);
    return *nodePtr;
}

} //namespace Cascade::NodeGraph
