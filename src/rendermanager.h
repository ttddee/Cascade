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

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QObject>

#include "nodebase.h"
#include "nodedefinitions.h"

class NodeGraph;

namespace Cascade::Renderer
{
    class VulkanRenderer;
}

using namespace Cascade::Renderer;

class RenderManager : public QObject
{
    Q_OBJECT

public:
    static RenderManager& getInstance();
    RenderManager(RenderManager const&) = delete;
    void operator=(RenderManager const&) = delete;

    void setUp(VulkanRenderer* r, NodeGraph* ng);

    void updateViewerPushConstants(const QString& s);

private:
    RenderManager() {}
    void displayNode(NodeBase* node);
    bool renderNodes(NodeBase* node);
    void renderNode(NodeBase* node);

    VulkanRenderer* renderer;
    NodeGraph* nodeGraph;

    WindowManager* wManager;

public slots:
    void handleNodeDisplayRequest(NodeBase* node);
    void handleNodeFileSaveRequest(
            NodeBase* node,
            const QString& path,
            const QMap<std::string, std::string>& attributes,
            const bool isBatch,
            const bool isLast);
    void handleClearScreenRequest();
};

#endif // RENDERMANAGER_H
