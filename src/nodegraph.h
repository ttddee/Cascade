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

using namespace Cascade;

class NodeGraph : public QGraphicsView
{
    Q_OBJECT

#ifdef QT_DEBUG
    friend class NodeBaseTest;
#endif

public:
    NodeGraph(QWidget* parent);

    void createNode(const NodeType type);
    void viewNode(NodeBase* node);
    void deleteNode(NodeBase* node);

    NodeBase* getViewedNode();
    NodeBase* getSelectedNode();

    float getViewScale() const;

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;

private:
    void showContextMenu(const QPoint& pos);

    QGraphicsItem* getObjectUnderCursor();
    QWidget* getWidgetFromGraphicsitem(QGraphicsItem* item);

    Connection* createOpenConnection(NodeOutput* nodeOut);
    void establishConnection(NodeInput* nodeIn);
    void destroyOpenConnection();
    void deleteConnection(Connection* c);

    void selectNode(NodeBase* node);
    void activateNode(NodeBase* node);

    QGraphicsScene* scene;
    WindowManager* wManager;
    RenderManager* rManager;
    NodeGraphContextMenu* contextMenu;

    std::vector<NodeBase*> nodes;
    std::vector<Connection*> connections;

    bool leftMouseIsDragging = false;
    bool middleMouseIsDragging = false;
    QPoint lastMousePos;

    float viewScale = 1.0f;

    NodeBase* selectedNode = nullptr;
    NodeBase* activeNode = nullptr;
    NodeBase* viewedNode = nullptr;
    Connection* openConnection = nullptr;

    const int viewWidth = 60000;
    const int viewHeight = 60000;

signals:
    void requestNodeDisplay(NodeBase* node);
    void requestNodeFileSave(NodeBase* node, const QString& path);
    void requestClearScreen();
    void requestClearProperties();

public slots:
    void handleNodeLeftClicked(NodeBase* node);
    void handleNodeDoubleClicked(NodeBase* node);
    void handleNodeOutputLeftClicked(NodeOutput* nodeOut);
    void handleNodeUpdateRequest(NodeBase* node);
    void handleFileSaveRequest(NodeBase* node, const QString& path);
    void handleConnectedNodeInputClicked(Connection* c);
};

#endif // NODEGRAPH_H
