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
        renderNode(n);
        std::cout << "rendering node" << std::endl;
    }
    renderNode(node);
}

void RenderManager::renderNode(NodeBase *node)
{
    //TODO: if needsupdate

    //auto props = node->getProperties();

//    if(node->nodeType == NODE_TYPE_READ)
//    {
//        std::cout << "This is a ReadNode" << std::endl;

//        QString path;

//        foreach(UiEntity* e, props->widgets)
//        {
//            if(e->elementType == UI_ELEMENT_TYPE_FILEBOX)
//            {
//                // TODO: This should be generalized
//                auto box = static_cast<FileBoxEntity*>(e);
//                path = box->getCurrentPath();

//            }
//        }
//        if(path != "")
//        {
//            renderer->updateImage(path);
//        }

//    }
//    else
//    {

    std::shared_ptr<CsImage> inputImage = nullptr;

    if (node->getUpstreamNode())
    {
        std::cout << "This node has an upstream node." << std::endl;
        inputImage = node->getUpstreamNode()->cachedImage;

        std::cout << node->getUpstreamNode()->cachedImage << std::endl;
    }


    renderer->processNode(node, *inputImage);
   // }
}
