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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

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

signals:
    void activeNodeChanged(Node* node);

public Q_SLOTS:
    void scaleUp();

    void scaleDown();

    void deleteSelectedNodes();

    void setActiveNode(Cascade::NodeGraph::Node* node);

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

    Node* mActiveNode = nullptr;
};

} // namespace Cascade::NodeGraph
