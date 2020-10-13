/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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
    void deviceLost();
    void rendererHasBeenCreated();
    void requestZoomTextUpdate(float f);
    void renderTargetHasBeenCreated(int w, int h);

public slots:
    void handleZoomResetRequest();

};

#endif // VULKANWINDOW_H
