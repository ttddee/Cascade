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

#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <set>

#include <QObject>
#include <QGraphicsView>

#include "nodedefinitions.h"
#include "nodebase.h"
#include "nodegraphcontextmenu.h"
#include "connection.h"
#include "../windowmanager.h"
#include "../rendermanager.h"
#include "nodegraphutility.h"
#include "nodegraphmodel.h"


namespace Cascade {

class NodeGraph : public QGraphicsView
{
    Q_OBJECT

friend class NodeBaseTest;

public:
    NodeGraph(QWidget* parent = nullptr);

    float getViewScale() const;

private:
    NodeGraphModel* mModel;

    // Graphics Scene
    QGraphicsItem* getObjectUnderCursor();
    QWidget* getWidgetFromGraphicsItem(QGraphicsItem* item);
    QPoint getCoordinatesForPosition(const NodeGraphPosition pos);

    // Misc
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void showContextMenu();

    QGraphicsScene* mScene;
    WindowManager* mWindowManager;
    RenderManager* mRenderManager;
    NodeGraphContextMenu* mContextMenu;

    // Mouse and zoom
    const int mViewWidth = 60000;
    const int mViewHeight = 60000;
    float mViewScale = 1.0f;
    bool mLeftMouseIsDragging = false;
    bool mMiddleMouseIsDragging = false;
    QPoint mLastMousePos;

signals:
    void requestNodeDisplay(Cascade::NodeBase* node);
    void requestNodeFileSave(
            Cascade::NodeBase* node,
            const QString& path,
            const QMap<std::string, std::string>& attributes,
            const bool isBatch = false,
            const bool isLast = false);
    void requestSetNodeSelected(Cascade::NodeBase* node, const bool b);
    void requestSetNodeActive(Cascade::NodeBase* node, const bool b);
    void requestSetNodeViewed(Cascade::NodeBase* node, const bool b);

    void requestClearScreen();
    void requestClearProperties();

    void projectIsDirty();

public slots:
    // Nodes
    void handleNodeCreationRequest(
            const Cascade::NodeType type,
            const Cascade::NodeGraphPosition pos,
            const QString& customName = "");
    void handleNodeLeftClicked(Cascade::NodeBase* node);
    void handleNodeDoubleClicked(Cascade::NodeBase* node);
    void handleNodeOutputLeftClicked(Cascade::NodeOutput* nodeOut);
    void handleNodeUpdateRequest(Cascade::NodeBase* node);
    void handleNodeDisplayRequest(Cascade::NodeBase* node);
    void handleConnectedNodeInputClicked(Cascade::Connection* c);
    void handleFileSaveRequest(
            Cascade::NodeBase* node,
            const QString& path,
            const QString& fileType,
            const QMap<std::string, std::string>& attributes,
            const bool batchRender);
    void handleSwitchToViewerMode(const Cascade::ViewerMode mode);

    // Key presses
    void handleDeleteKeyPressed();

    // Project management
    void handleCreateStartupProject();
    void handleCreateNewProject();
    void handleLoadProject(const QJsonArray& jsonNodeGraph);

    // Application
    void handleShutdownRequest();
};

} // namespace Cascade

#endif // NODEGRAPH_H
