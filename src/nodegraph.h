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

public:
    NodeGraph(QWidget* parent);

    void createNode(const NodeType type);
    std::set<NodeBase*> getAllUpstreamNodes(NodeBase* node);
    void viewNode(NodeBase* node);

    float getViewScale() const;
    NodeBase* getSelectedNode();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    //void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent*) override;

private:
    void showContextMenu(const QPoint& pos);
    QGraphicsItem* getObjectUnderCursor();
    QWidget* getWidgetFromGraphicsitem(QGraphicsItem* item);
    void createOpenConnection(NodeOutput* nodeOut);
    void destroyOpenConnection();
    void establishConnection(NodeInput* nodeIn);

    void selectNode(NodeBase* node);
    void activateNode(NodeBase* node);

    QGraphicsScene* scene;
    WindowManager* wManager;
    RenderManager* rManager;
    const int viewWidth = 60000;
    const int viewHeight = 60000;
    std::vector<NodeBase*> nodes;
    std::vector<Connection*> connections;
    NodeGraphContextMenu* contextMenu;
    bool leftMouseIsDragging = false;
    bool middleMouseIsDragging = false;
    QPoint lastMousePos;
    float viewScale = 1.0f;

    NodeBase* selectedNode = nullptr;
    NodeBase* activeNode = nullptr;
    NodeBase* viewedNode = nullptr;
    Connection* openConnection = nullptr;

signals:
    void viewedNodeHasChanged(NodeBase* node);
    void requestNodeDisplay(NodeBase* node);
    void requestNodeFileSave(NodeBase* node, const QString& path);

public slots:
    void handleNodeLeftClicked(NodeBase* node);
    void handleNodeDoubleClicked(NodeBase* node);
    void handleNodeOutputLeftClicked(NodeOutput* nodeOut);
    void handleNodeUpdateRequest(NodeBase* node);
    void handleFileSaveRequest(NodeBase* node, const QString& path);
};

#endif // NODEGRAPH_H
