/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#include <iostream>

#include <QFile>
#include <QMessageBox>

#include "uientity.h"
#include "fileboxentity.h"
#include "vulkanrenderer.h"

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
    auto props = getPropertiesForType(node->nodeType);

    auto viewerMode = wManager->getViewerMode();

    renderer->setDisplayMode(DISPLAY_MODE_RGB);

    NodeBase* nodeToDisplay = nullptr;

    if (viewerMode == VIEWER_MODE_FRONT_RGB)
    {
        if (props.frontInputTrait == FRONT_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.frontInputTrait == FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            nodeToDisplay = node->getUpstreamNodeFront();
            displayNode(nodeToDisplay);
        }
    }
    else if (viewerMode == VIEWER_MODE_BACK_RGB)
    {
        if (props.backInputTrait == BACK_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.backInputTrait == BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            nodeToDisplay = node->getUpstreamNodeBack();
            displayNode(nodeToDisplay);
        }
    }
    else if (viewerMode == VIEWER_MODE_BACK_ALPHA)
    {
        if (props.backInputTrait == BACK_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.backInputTrait == BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            nodeToDisplay = node->getUpstreamNodeBack();
            renderer->setDisplayMode(DISPLAY_MODE_ALPHA);
            displayNode(nodeToDisplay);
        }
    }
    else if (viewerMode == VIEWER_MODE_OUTPUT_RGB)
    {
        displayNode(node);
    }
    else if (viewerMode == VIEWER_MODE_OUTPUT_ALPHA)
    {
        renderer->setDisplayMode(DISPLAY_MODE_ALPHA);
        displayNode(node);
    }
}

void RenderManager::handleNodeFileSaveRequest(NodeBase* node, const QString& path)
{
    if (node->canBeRendered())
    {
        auto upstream = node->getUpstreamNodeBack();
        auto image = upstream->cachedImage;
        if(image)
        {
            if(renderer->saveImageToDisk(*image, path))
            {
                QMessageBox messageBox;
                messageBox.information(0,"Success","File saved successfully.");
            }
            else
            {
                QMessageBox messageBox;
                messageBox.information(0,"Error","There was a problem saving the file.");
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
        {
            renderer->displayNode(node);
        }
        else
        {
            renderer->doClearScreen();
        }
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
        {
            renderNode(n);
        }
        else
        {
            allNodesRendered = false;
        }
    }

    return allNodesRendered;
}

void RenderManager::renderNode(NodeBase *node)
{
    if (node->nodeType == NODE_TYPE_READ && node->needsUpdate)
    {
        if (node->canBeRendered())
        {
            renderer->processReadNode(node);
        }
    }
    else if (node->getUpstreamNodeBack() && node->needsUpdate)
    {
        std::cout << "Rendering node" << std::endl;

        std::shared_ptr<CsImage> inputImageBack = nullptr;
        std::shared_ptr<CsImage> inputImageFront = nullptr;

        inputImageBack = node->getUpstreamNodeBack()->cachedImage;

        if(node->getUpstreamNodeFront() && node->getUpstreamNodeFront()->cachedImage)
        {
            std::cout << "Rendering two inputs" << std::endl;
            inputImageFront = node->getUpstreamNodeFront()->cachedImage;
            renderer->processNode(node, *inputImageBack, *inputImageFront, node->getTargetSize());
        }
        else if (inputImageBack)
        {
            std::cout << "Rendering one input" << std::endl;
            renderer->processNode(node, *inputImageBack, node->getTargetSize());
        }
    }
    node->needsUpdate = false;
}
