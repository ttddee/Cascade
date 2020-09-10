#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QObject>
#include <QGraphicsView>

#include "nodedefinitions.h"
#include "nodebase.h"
#include "nodegraphcontextmenu.h"
#include "connection.h"
#include "windowmanager.h"

using namespace Cascade;

class NodeGraph : public QGraphicsView
{
    Q_OBJECT

public:
    NodeGraph(QWidget* parent);
    void createNode(const NodeType type);
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
    void createOpenConnection(NodeOutput* nodeOut);
    void destroyOpenConnection();
    void establishConnection(NodeInput* nodeIn);

    QGraphicsScene* scene;
    WindowManager* wManager;
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

public slots:
    void handleNodeLeftClicked(NodeBase* node);
    void handleNodeDoubleClicked(NodeBase* node);
    void handleNodeOutputLeftClicked(NodeOutput* nodeOut);
};

#endif // NODEGRAPH_H
