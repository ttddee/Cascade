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
    else if (viewerMode == VIEWER_MODE_INPUT_ALPHA)
    {
        if (props.alphaInputTrait == ALPHA_INPUT_ALWAYS_CLEAR)
        {
            renderer->doClearScreen();
        }
        else if (props.alphaInputTrait == ALPHA_INPUT_RENDER_UPSTREAM_OR_CLEAR)
        {
            nodeToDisplay = node->getUpstreamNodeFront();
            renderer->setDisplayMode(DISPLAY_MODE_ALPHA);
            displayNode(nodeToDisplay);
        }
    }
    else if (viewerMode == VIEWER_MODE_OUTPUT_RGB || viewerMode == VIEWER_MODE_OUTPUT_ALPHA)
    {
        nodeToDisplay = node;

        if (viewerMode == VIEWER_MODE_OUTPUT_ALPHA)
        {
            renderer->setDisplayMode(DISPLAY_MODE_ALPHA);
        }
        if (props.rgbOutputTrait == OUTPUT_RENDER_UPSTREAM_IF_FRONT_DISCONNECTED)
        {
            if (!node->getUpstreamNodeFront())
            {
                nodeToDisplay = node->getUpstreamNodeBack();
            }
        }
        displayNode(nodeToDisplay);
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
            auto parts = node->getAllPropertyValues().split(",");

            if(renderer->saveImageToDisk(*image, path, parts.last().toInt()))
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
    if (node->nodeType == NODE_TYPE_READ)
    {
        if(node->needsUpdate)
        {
            renderer->processReadNode(node);
        }
        // TODO: This is inefficient, better to display upon render
        renderer->displayNode(node);
        node->needsUpdate = false;
    }
    if (node->nodeType == NODE_TYPE_GMIC)
    {
        if(node->needsUpdate)
        {
            if (node->canBeRendered())
            {
                auto inputImage = node->getUpstreamNodeBack()->cachedImage;
                renderer->processGmicNode(node, inputImage, node->getTargetSize());
                renderer->displayNode(node);
                node->needsUpdate = false;
            }
        }
        // TODO: This is inefficient, better to display upon render

    }
    else if (node && node->canBeRendered())
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
    if (node->getHasCustomSize() && node->getCustomSize() != "" && node->needsUpdate)
    {
        renderer->processNode(node, nullptr, nullptr, node->getTargetSize());
    }
    if (node->getUpstreamNodeBack() && node->needsUpdate)
    {
        std::shared_ptr<CsImage> inputImageBack = nullptr;
        std::shared_ptr<CsImage> inputImageFront = nullptr;

        inputImageBack = node->getUpstreamNodeBack()->cachedImage;

        if(node->getUpstreamNodeFront() && node->getUpstreamNodeFront()->cachedImage)
        {
            std::cout << "Rendering two inputs" << std::endl;
            inputImageFront = node->getUpstreamNodeFront()->cachedImage;
            renderer->processNode(node, inputImageBack, inputImageFront, node->getTargetSize());
        }
        else if (inputImageBack)
        {
            if (node->nodeType == NODE_TYPE_GMIC)
            {
                renderer->processGmicNode(node, inputImageBack, node->getTargetSize());
            }

            {
                renderer->processNode(node, inputImageBack, nullptr, node->getTargetSize());
            }
        }
    }
    node->needsUpdate = false;
}
