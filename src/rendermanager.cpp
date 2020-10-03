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
    if (node->getUpstreamNode() || node->nodeType == NODE_TYPE_READ)
    {
        renderNodes(node);
    }
}

void RenderManager::renderNodes(NodeBase *node)
{
    auto nodes = node->getAllUpstreamNodes();
    foreach(NodeBase* n, nodes)
    {
        renderNode(n);
    }
}

void RenderManager::renderNode(NodeBase *node)
{
    //auto viewerMode = wManager->getViewerMode();

    if(node->needsUpdate)
    {
        std::cout << "rendering node" << std::endl;

        std::shared_ptr<CsImage> inputImage = nullptr;

        if (node->getUpstreamNode())
        {
            inputImage = node->getUpstreamNode()->cachedImage;
        }

        renderer->processNode(node, *inputImage, node->getTargetSize());

        node->needsUpdate = false;
    }
    else if (node->getIsViewed())
    {
        std::cout << "displaying node without rendering" << std::endl;

        renderer->displayProcessedNode(node);
    }

}
