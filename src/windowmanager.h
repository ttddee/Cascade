#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include "nodebase.h"

class VulkanWindow;
class NodeGraph;
class PropertiesView;
class ViewerStatusBar;

enum ViewerMode
{
    VIEWER_MODE_FRONT,
    VIEWER_MODE_BACK,
    VIEWER_MODE_ALPHA,
    VIEWER_MODE_OUTPUT
};

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

    bool eventFilter(QObject* watched, QEvent* event);

    VulkanWindow* vulkanWindow;
    NodeGraph* nodeGraph;
    PropertiesView* propertiesView;
    ViewerStatusBar* viewerStatusBar;

    ViewerMode currentViewerMode;

public slots:
    void handleNodeDoubleClicked(NodeBase* node);
    void handleZoomTextUpdateRequest(float f);
    void handleRenderTargetCreated(int w, int h);
};

#endif // WINDOWMANAGER_H
