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
    renderNode(node);
}

void RenderManager::renderNode(NodeBase *node)
{
    auto props = node->getProperties();

    if(node->nodeType == NODE_TYPE_READ)
    {
        QString path;

        foreach(UiEntity* e, props->widgets)
        {
            if(e->elementType == UI_ELEMENT_TYPE_FILEBOX)
            {
                auto box = static_cast<FileBoxEntity*>(e);
                path = box->getCurrentPath();

            }
        }
        std::cout << path.toStdString() << std::endl;
        if(path != "")
        {
            renderer->updateImage(path);
        }

    }



}
