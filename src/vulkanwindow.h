#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QObject>
#include <QVulkanWindow>
#include <QWindow>

#include "windowmanager.h"

class VulkanRenderer;

class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT

public:
    QVulkanWindowRenderer *createRenderer() override;

    VulkanRenderer* getRenderer();

    ViewerMode getViewerMode();
    void setViewerMode(const ViewerMode mode);

private:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    VulkanRenderer* renderer;
    bool pressed = false;
    QPoint lastPos;
    float scale = 1.0f;
    const float minScale = 0.1f;
    const float maxScale = 10.0f;
    ViewerMode viewerMode;

signals:
    void noGPUFound();
    void rendererHasBeenCreated();
    void requestZoomTextUpdate(float f);
    void renderTargetHasBeenCreated(int w, int h);

public slots:
    void handleZoomResetRequest();

};

#endif // VULKANWINDOW_H
