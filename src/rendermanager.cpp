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

#include "rendermanager.h"

#include <QFile>

#include "uientities/uientity.h"
#include "uientities/fileboxentity.h"
#include "renderer/vulkanrenderer.h"
#include "popupmessages.h"

namespace Cascade {

RenderManager& RenderManager::getInstance()
{
    static RenderManager instance;

    return instance;
}

void RenderManager::setUp(VulkanRenderer *r, NodeGraph* ng)
{
    renderer = r;
    nodeGraph = ng;

    wManager = &WindowManager::getInstance();
}

void RenderManager::updateViewerPushConstants(const QString &s)
{
    renderer->setViewerPushConstants(s);
}

void RenderManager::handleNodeDisplayRequest(NodeBase* node)
{
    auto props = getPropertiesForType(node->getType());

    auto viewerMode = wManager->getViewerMode();

    renderer->setDisplayMode(DisplayMode::eRgb);

    if (viewerMode == ViewerMode::eFrontRgb)
    {
        if (props.frontInputTrait == FRONT_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.frontInputTrait == FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            displayNode(node->getUpstreamNodeFront());
        }
    }
    else if (viewerMode == ViewerMode::eBackRgb)
    {
        if (props.backInputTrait == BACK_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.backInputTrait == BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            displayNode(node->getUpstreamNodeBack());
        }
    }
    else if (viewerMode == ViewerMode::eInputAlpha)
    {
        if (props.alphaInputTrait == ALPHA_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.alphaInputTrait == ALPHA_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            renderer->setDisplayMode(DisplayMode::eAlpha);
            displayNode(node->getUpstreamNodeFront());
        }
    }
    else if (viewerMode == ViewerMode::eOutputRgb || viewerMode == ViewerMode::eOutputAlpha)
    {
        NodeBase* nodeToDisplay = node;

        if (viewerMode == ViewerMode::eOutputAlpha)
        {
            renderer->setDisplayMode(DisplayMode::eAlpha);
        }
        if (props.rgbOutputTrait == OUTPUT_RENDER_UPSTREAM_IF_FRONT_DISCONNECTED)
        {
            if (!node->getUpstreamNodeFront())
            {
                if (node->getUpstreamNodeBack())
                {
                    nodeToDisplay = node->getUpstreamNodeBack();
                }
            }
        }
        displayNode(nodeToDisplay);
    }
}

void RenderManager::handleNodeFileSaveRequest(
        NodeBase* node,
        const QString& path,
        const QMap<std::string, std::string>& attributes,
        const bool isBatch,
        const bool isLast)
{
    if (node->canBeRendered())
    {
        auto upstream = node->getUpstreamNodeBack();
        auto image = upstream->getCachedImage();
        if(image)
        {
            auto parts = node->getAllPropertyValues().split(",");

//            auto handle = std::async(std::launch::async, [this, image, path, parts]()
//            {
//                return renderer->saveImageToDisk(image, path, parts.last().toInt());
//            });
//            bool success = handle.get();

            if(renderer->saveImageToDisk(image, path, attributes, parts.last().toInt()))
            {
                if (!isBatch)
                {
                    executeMessageBox(MESSAGEBOX_FILE_SAVE_SUCCESS);
                }
                if (isBatch && isLast)
                {
                    executeMessageBox(MESSAGEBOX_FILES_SAVE_SUCCESS);
                }
            }
            else
            {
                executeMessageBox(MESSAGEBOX_FILE_SAVE_PROBLEM);
            }
        }
    }
}

void RenderManager::handleClearScreenRequest()
{
    renderer->doClearScreen();
}

void RenderManager::displayNode(NodeBase* node)
{
    if (node && node->canBeRendered())
    {
        if (renderNodes(node))
            renderer->displayNode(node);
        else
            renderer->doClearScreen();
    }
    else
    {
        renderer->doClearScreen();
    }
}

bool RenderManager::renderNodes(NodeBase *node)
{
    bool allNodesRendered = true;

    std::vector<NodeBase*> nodes;
    node->getAllUpstreamNodes(nodes);

    foreach(NodeBase* n, nodes)
    {
        if (n->canBeRendered())
            renderNode(n);
        else
            allNodesRendered = false;
    }

    return allNodesRendered;
}

void RenderManager::renderNode(NodeBase *node)
{
    // Read node
    if (node->getType() == NODE_TYPE_READ && node->getNeedsUpdate())
    {
        if (node->canBeRendered())
        {
            renderer->processReadNode(node);
        }
    }
    // All other nodes
    else if (node->getUpstreamNodeBack() && node->getNeedsUpdate())
    {
        CsImage* inputImageBack = nullptr;
        CsImage* inputImageFront = nullptr;

        inputImageBack = node->getUpstreamNodeBack()->getCachedImage();

        // A node that has a front and back image
        if(node->getUpstreamNodeFront() && node->getUpstreamNodeFront()->getCachedImage())
        {
            inputImageFront = node->getUpstreamNodeFront()->getCachedImage();
            renderer->processNode(node, inputImageBack, inputImageFront, node->getTargetSize());
        }
        // A node without a front image
        else if (inputImageBack)
        {
            renderer->processNode(node, inputImageBack, nullptr, node->getTargetSize());
        }
    }
    node->setNeedsUpdate(false);
}

} // namespace Cascade
