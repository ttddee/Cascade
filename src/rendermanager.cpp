#include "rendermanager.h"

#include <iostream>

#include <QFile>

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
            renderer->saveImageToDisk(*image, path);
        }
    }
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

    auto nodes = node->getAllUpstreamNodes();
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
        renderer->processReadNode(node);
    }
    else if (node->getUpstreamNodeBack() && node->needsUpdate)
    {
        std::cout << "rendering node" << std::endl;

        std::shared_ptr<CsImage> inputImageBack = nullptr;
        std::shared_ptr<CsImage> inputImageFront = nullptr;

        inputImageBack = node->getUpstreamNodeBack()->cachedImage;

        if(node->getUpstreamNodeFront() && node->getUpstreamNodeFront()->cachedImage)
        {
            std::cout << "Rendering two inputs" << std::endl;
            inputImageFront = node->getUpstreamNodeFront()->cachedImage;
            renderer->processNode(node, *inputImageBack, *inputImageFront, node->getTargetSize());
        }
        else
        {
            std::cout << "Rendering one input" << std::endl;
            renderer->processNode(node, *inputImageBack, node->getTargetSize());
        }
    }
    node->needsUpdate = false;
}
