#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <QObject>
#include <QGraphicsView>

#include "nodedefinitions.h"
#include "nodebase.h"
#include "nodegraphcontextmenu.h"

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

    QGraphicsScene* scene;
    const int viewWidth = 60000;
    const int viewHeight = 60000;
    std::vector<NodeBase*> nodes;
    NodeGraphContextMenu* contextMenu;
    bool isDragging = false;
    QPoint lastMousePos;
    float viewScale = 1.0f;

    NodeBase* selectedNode;
    NodeBase* activeNode;
    NodeBase* viewedNode;

public slots:
    void handleNodeWasLeftClicked(NodeBase* node);
    void handleNodeWasDoubleClicked(NodeBase* node);
};

#endif // NODEGRAPH_H
