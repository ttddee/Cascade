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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include "global.h"

class VulkanWindow;
class NodeGraph;
class PropertiesView;
class ViewerStatusBar;
class NodeBase;
class RenderManager;

using namespace Cascade;

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

    ViewerMode getViewerMode();

private:
    WindowManager() {}

    bool eventFilter(QObject* watched, QEvent* event);

    void setViewerMode(const ViewerMode mode);

    VulkanWindow* vulkanWindow;
    NodeGraph* nodeGraph;
    PropertiesView* propertiesView;
    ViewerStatusBar* viewerStatusBar;

    RenderManager* rManager;

    ViewerMode currentViewerMode = VIEWER_MODE_OUTPUT_RGB;

public slots:
    void handleNodeDoubleClicked(NodeBase* node);
    void handleZoomTextUpdateRequest(float f);
    void handleRenderTargetCreated(int w, int h);
    void handleViewerStatusBarValueChanged();
    void handleClearPropertiesRequest();
    void handleViewerModeChanged(const Cascade::ViewerMode mode);
};

#endif // WINDOWMANAGER_H
