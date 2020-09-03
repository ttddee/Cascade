#include "nodegraph.h"

#include <iostream>

#include <QMouseEvent>
#include <QScrollBar>

NodeGraph::NodeGraph(QWidget* parent)
    : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    this->setScene(scene);

    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);
    this->setSceneRect(0, 0, viewWidth, viewHeight);
    this->centerOn(viewWidth / 2, viewHeight / 2);

    contextMenu = new NodeGraphContextMenu(this);
}

void NodeGraph::createNode(const NodeType type)
{
    NodeBase* n = new NodeBase(type, this);
    scene->addWidget(n);
    n->move(mapToScene(lastMousePos).x(),
            mapToScene(lastMousePos).y());
    nodes.push_back(n);

    std::cout << " NODE X: " << mapToScene(lastMousePos).x() << std::endl;
    std::cout << " NODE Y: " << mapToScene(lastMousePos).y() << std::endl;

    connect(n, &NodeBase::nodeWasLeftClicked,
                this, &NodeGraph::handleNodeLeftMouseClicked);
    connect(n, &NodeBase::nodeWasDoubleClicked,
            this, &NodeGraph::handleNodeMouseDoubleClicked);
}

float NodeGraph::getViewScale() const
{
    return viewScale;
}

void NodeGraph::showContextMenu(const QPoint &pos)
{
    contextMenu->exec(mapToGlobal(pos));
}

void NodeGraph::handleNodeLeftMouseClicked(NodeBase* node)
{
    selectedNode = node;
    foreach(NodeBase* n, nodes)
    {
        n->setIsSelected(false);
    }
    node->setIsSelected(true);
}

void NodeGraph::handleNodeMouseDoubleClicked(NodeBase* node)
{
    if (node)
    {
        activeNode = node;
        node->setIsActive(true);
        //m_properties->loadNodeProperties(node); // USE A SIGNAL
    }
    else
    {
        activeNode = nullptr;
        //m_properties->clear(); // USE A SIGNAL
    }
}

void NodeGraph::handleNodeOutputLeftMouseClicked(NodeOutput* nodeOut)
{
    leftMouseIsDragging = true;

    Connection* c = new Connection(nodeOut);
    openConnection = c;
    scene->addItem(openConnection);

    std::cout << "Creating connection" << std::endl;
}

void NodeGraph::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        showContextMenu(event->pos());
    }
    else if (event->button() == Qt::MiddleButton)
    {
        middleMouseisDragging = true;
    }
    lastMousePos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void NodeGraph::mouseMoveEvent(QMouseEvent* event)
{
    if (middleMouseisDragging)
    {
        auto t = event->pos() - lastMousePos;
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() - t.x());
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - t.y());
    }
    else if(leftMouseIsDragging)
    {
        if(openConnection)
        {
            openConnection->updatePosition(QPoint(
                                           mapToScene(mapFromGlobal(QCursor::pos())).x(),
                                           mapToScene(mapFromGlobal(QCursor::pos())).y()));

            scene->update();
        }
    }
    lastMousePos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
}

void NodeGraph::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftMouseIsDragging = false;
    }
    if (event->button() == Qt::MiddleButton)
        middleMouseisDragging = false;
    QGraphicsView::mouseReleaseEvent(event);
}

void NodeGraph::wheelEvent(QWheelEvent* event)
{
    QPoint scrollAmount = event->angleDelta();
    double factor = (scrollAmount.y() > 0) ? 1.2 : 1 / 1.2;
    this->scale(factor, factor);
    viewScale *= factor;
}
