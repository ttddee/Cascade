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
    if (wManager->getViewerMode() == VIEWER_MODE_FRONT)
    {
        if (auto upstream = node->getUpstreamNodeFront())
        {
            if (upstream->canBeRendered())
            {
                renderNodes(upstream);
                renderer->displayNode(upstream);

                return;
            }
        }
        renderer->doClearScreen();
    }
    else if (wManager->getViewerMode() == VIEWER_MODE_BACK)
    {
        if (auto upstream = node->getUpstreamNodeBack())
        {
            if (upstream->canBeRendered())
            {
                renderNodes(upstream);
                renderer->displayNode(upstream);

                return;
            }
        }
        renderer->doClearScreen();
    }
    // Viewer Modes Alpha and Output are handled in VulkanRenderer
    else
    {
        if (!node->canBeRendered())
        {
            renderer->doClearScreen();
        }
        else
        {
            if (node->needsUpdate)
            {
                if (node->nodeType == NODE_TYPE_READ)
                {
                    renderer->processReadNode(node);
                }
                else
                {
                    renderNodes(node);
                }
            }
            renderer->displayNode(node);

            node->needsUpdate = false;
        }
    }
}

void RenderManager::renderNodes(NodeBase *node)
{
    auto nodes = node->getAllUpstreamNodes();
    foreach(NodeBase* n, nodes)
    {
        if (node->nodeType == NODE_TYPE_READ)
        {
            renderer->processReadNode(node);
        }
        else if (n->needsUpdate && node->getUpstreamNodeBack())
        {
            renderNode(n);
        }
        else if(!node->getUpstreamNodeBack())
        {
            renderer->doClearScreen();

            return;
        }
    }
}

void RenderManager::renderNode(NodeBase *node)
{
    std::cout << "rendering node" << std::endl;

    std::shared_ptr<CsImage> inputImageBack = nullptr;
    std::shared_ptr<CsImage> inputImageFront = nullptr;

    inputImageBack = node->getUpstreamNodeBack()->cachedImage;

    if(node->getUpstreamNodeFront())
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
