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
}

void RenderManager::handleViewedNodeHasChanged(NodeBase* node)
{
    std::cout << "viewedChanged" << std::endl;
    renderNodes(node);

}

void RenderManager::renderNodes(NodeBase *node)
{
    auto nodes = node->getAllUpstreamNodes();
    foreach(NodeBase* n, nodes)
    {
        std::cout << "rendering node" << std::endl;
        renderNode(n);
    }
    renderNode(node);
}

void RenderManager::renderNode(NodeBase *node)
{
    if(node->needsUpdate)
    {
        std::shared_ptr<CsImage> inputImage = nullptr;

        if (node->getUpstreamNode())
        {
            inputImage = node->getUpstreamNode()->cachedImage;
        }
        renderer->processNode(node, *inputImage);

        node->needsUpdate = false;
    }
}
