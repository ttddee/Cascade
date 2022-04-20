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
#include "windowmanager.h"
#include "rendermanager.h"

namespace Cascade {

struct NodePersistentProperties
{
    NodeType mNodeType;
    QPoint mPos;
    QString mUuid;
    QMap<int, QString> mInputs;
    QMap<int, QString> mProperties;
    QString mCustomName;
};

class NodeGraph : public QGraphicsView
{
    Q_OBJECT

friend class NodeBaseTest;

public:
    NodeGraph(QWidget* parent = nullptr);

    void createNode(
            const NodeType type,
            const QPoint pos,
            const bool view = true,
            const QString& customName = "");
    void viewNode(NodeBase* node);

    NodeBase* getViewedNode();
    NodeBase* getSelectedNode();
    const QPoint getLastMousePosition();
    const QPoint getLastCreatedNodePosition();

    float getViewScale() const;

    void getNodeGraphAsJson(QJsonArray& jsonNodeGraph);

private:
    // Project
    void createProject();
    void loadProject(const QJsonArray& jsonNodeGraph);
    void clearGraph();

    // Nodes
    void deleteNode(NodeBase* node);
    void selectNode(NodeBase* node);
    void activateNode(NodeBase* node);
    NodeBase* findNodeById(const QString& id);
    NodeBase* loadNode(const NodePersistentProperties& p);
    void connectNodeSignals(NodeBase* n);
    void flushCacheAllNodes();

    // Connections
    Connection* createOpenConnection(NodeOutput* nodeOut);
    void establishConnection(NodeInput* nodeIn);
    void destroyOpenConnection();
    void deleteConnection(Connection* c);
    void loadConnection(NodeOutput* src, NodeInput* dst);

    // Graphics Scene
    QGraphicsItem* getObjectUnderCursor();
    QWidget* getWidgetFromGraphicsItem(QGraphicsItem* item);

    // Misc
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void showContextMenu();

    ////////////////////////////////////////
    QGraphicsScene* mScene;
    WindowManager* mWindowManager;
    RenderManager* mRenderManager;
    NodeGraphContextMenu* mContextMenu;

    std::vector<NodeBase*> mNodes;
    std::vector<Connection*> mConnections;

    // The selected node
    NodeBase* mSelectedNode = nullptr;
    // The node with active properties
    NodeBase* mActiveNode = nullptr;
    // The node that is being displayed
    NodeBase* mViewedNode = nullptr;

    // Mouse and zoom
    const int mViewWidth = 60000;
    const int mViewHeight = 60000;
    float mViewScale = 1.0f;
    bool mLeftMouseIsDragging = false;
    bool mMiddleMouseIsDragging = false;
    QPoint mLastMousePos;
    QPoint mLastCreatedNodePos = QPoint(29700, 29920);

    Connection* mOpenConnection = nullptr;

signals:
    void requestNodeDisplay(Cascade::NodeBase* node);
    void requestNodeFileSave(
            Cascade::NodeBase* node,
            const QString& path,
            const QMap<std::string, std::string>& attributes,
            const bool isBatch = false,
            const bool isLast = false);
    void requestClearScreen(); // Check this
    void requestClearProperties();
    void projectIsDirty();

public slots:
    // Nodes
    void handleNodeLeftClicked(Cascade::NodeBase* node);
    void handleNodeDoubleClicked(Cascade::NodeBase* node);
    void handleNodeOutputLeftClicked(Cascade::NodeOutput* nodeOut);
    void handleNodeUpdateRequest(Cascade::NodeBase* node);
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
