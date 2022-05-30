#pragma once

#include <QGraphicsView>

#include "datamodelregistry.h"

#include "nodes/testdatamodel.h"

namespace Cascade::NodeGraph
{

class NodeGraphScene;
class ContextMenu;

class NodeGraphView : public QGraphicsView
{
    Q_OBJECT

public:
    NodeGraphView(QWidget *parent = Q_NULLPTR);

    NodeGraphView(const NodeGraphView&) = delete;
    NodeGraphView operator=(const NodeGraphView&) = delete;

    QAction* clearSelectionAction() const;

    QAction* deleteSelectionAction() const;

    void setScene(NodeGraphScene *scene);

public Q_SLOTS:
    void scaleUp();

    void scaleDown();

    void deleteSelectedNodes();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void drawBackground(QPainter* painter, const QRectF& r) override;

    void showEvent(QShowEvent* event) override;

protected:
    NodeGraphScene* scene();

private:
    std::shared_ptr<DataModelRegistry> registerDataModels()
    {
        auto ret = std::make_shared<DataModelRegistry>();
        ret->registerModel<TestDataModel>("Test");

        return ret;
    }

    QAction* mClearSelectionAction;
    QAction* mDeleteSelectionAction;

    QPointF mMiddleClickPos;

    NodeGraphScene* mScene;
    ContextMenu* mContextMenu;
};

} // namespace Cascade::NodeGraph
