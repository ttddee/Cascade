#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include "nodebase.h"

class VulkanWindow;
class NodeGraph;
class PropertiesView;
class ViewerStatusBar;

class WindowManager : public QObject
{
    Q_OBJECT

public:
    static WindowManager& getInstance();
    WindowManager(WindowManager const&) = delete;
    void operator=(WindowManager const&) = delete;
    void setUp(VulkanWindow* vw,
               NodeGraph* ng,
               PropertiesView* pv,
               ViewerStatusBar* vb);

private:
    WindowManager() {}

    VulkanWindow* vulkanWindow;
    NodeGraph* nodeGraph;
    PropertiesView* propertiesView;
    ViewerStatusBar* viewerStatusBar;

signals:

public slots:
    void handleNodeDoubleClicked(NodeBase* node);
    void handleZoomTextUpdateRequest(float f);
    void handleRenderTargetCreated(int w, int h);
};

#endif // WINDOWMANAGER_H
