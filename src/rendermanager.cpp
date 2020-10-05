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
    if (node->nodeType != NODE_TYPE_READ && !node->getUpstreamNode())
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

void RenderManager::renderNodes(NodeBase *node)
{
    auto nodes = node->getAllUpstreamNodes();
    foreach(NodeBase* n, nodes)
    {
        if (node->nodeType == NODE_TYPE_READ)
        {
            renderer->processReadNode(node);
        }
        else if (n->needsUpdate && node->getUpstreamNode())
        {
            renderNode(n);
        }
        else if(!node->getUpstreamNode())
        {
            renderer->doClearScreen();

            return;
        }
    }
}

void RenderManager::renderNode(NodeBase *node)
{
    std::cout << "rendering node" << std::endl;

    std::shared_ptr<CsImage> inputImage = nullptr;

    inputImage = node->getUpstreamNode()->cachedImage;

    renderer->processNode(node, *inputImage, node->getTargetSize());

}
