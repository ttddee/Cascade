#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QObject>

#include "nodebase.h"
#include "nodedefinitions.h"

class VulkanRenderer;
class NodeGraph;

class RenderManager : public QObject
{
    Q_OBJECT

public:
    static RenderManager& getInstance();
    RenderManager(RenderManager const&) = delete;
    void operator=(RenderManager const&) = delete;
    void setUp(VulkanRenderer* r, NodeGraph* ng);

private:
    RenderManager() {}
    void renderNode(NodeBase* node);

    VulkanRenderer* renderer;
    NodeGraph* nodeGraph;

signals:

public slots:
    void handleViewedNodeHasChanged(NodeBase* node);
};

#endif // RENDERMANAGER_H
