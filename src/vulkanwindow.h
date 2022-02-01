/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QObject>
#include <QVulkanWindow>
#include <QWindow>

#include "windowmanager.h"
#include "global.h"

namespace Cascade::Renderer
{
    class VulkanRenderer;
}

using namespace Cascade::Renderer;

class VulkanWindow : public QVulkanWindow
{
    Q_OBJECT

public:
    QVulkanWindowRenderer *createRenderer() override;

    VulkanRenderer* getRenderer();

    ViewerMode getViewerMode();
    void setViewerMode(const ViewerMode mode);

    ~VulkanWindow();

private:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void wheelEvent(QWheelEvent *) override;

    VulkanRenderer* renderer;

    bool middleIsDragging = false;

    QPoint lastPos;

    float zoomFactor = 1.0f;
    const float minZoom = 0.1f;
    const float maxZoom = 10.0f;

    ViewerMode viewerMode;

signals:
    void noGPUFound();
    void deviceLost();
    void rendererHasBeenCreated();
    void requestZoomTextUpdate(float f);
    void renderTargetHasBeenCreated(int w, int h);
    void viewerHasBeenClicked(QPoint pos);

public slots:
    void handleZoomResetRequest();

};

#endif // VULKANWINDOW_H
