#include "nodebase.h"
#include "ui_nodebase.h"

#include <QPainter>
#include <QMouseEvent>

#include "nodeinput.h"
#include "nodeoutput.h"
#include "nodegraph.h"

NodeBase::NodeBase(const NodeType type, const NodeGraph* graph, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::NodeBase),
      nodeType(type),
      nodeGraph(graph)
{
    ui->setupUi(this);

    this->setUpNode(type);
}

void NodeBase::setUpNode(const NodeType nodeType)
{
    NodeInitProperties props;

    if (nodeType == NODE_TYPE_READ)
    {
        props = readNodeInitProperties;
    }

    ui->NodeTitleLabel->setText(props.title);

    this->createInputs(props);
    this->createOutputs(props);
}

void NodeBase::createInputs(const NodeInitProperties &props)
{
    // Create inputs
    for (size_t i = 0; i < props.nodeInputs.size(); i++)
    {
        auto nodeIn = new NodeInput(this);
        nodeIn->move(-2, 15);
        //inAnchorPos = QPoint(nodeIn->pos().x() + nodeIn->width() / 2,
        //                     nodeIn->pos().y() + nodeIn->height() / 2);
        nodeInputs.push_back(nodeIn);

        if (props.nodeInputs[i] == NODE_INPUT_TYPE_RGB_BACK)
        {
            nodeIn->setObjectName("RGBBackIn");
            this->rgbBackIn = nodeIn;
        }

//        connect(nodeIn, &NodeIn::mouseRealeasedOnNodeIn,
//                        controller, &Controller::handleMouseReleasedOnNodeIn);
//        connect(nodeIn, &NodeIn::nodeInReceivedInConnection,
//                    this, &NodeBase::handleNodeInReceivedInConnection);
//        connect(nodeIn, &NodeIn::nodeInLostInConnection,
//                    this, &NodeBase::handleNodeInLostInConnection);
    }
}

void NodeBase::createOutputs(const NodeInitProperties &props)
{
    // Create outputs
    for (size_t i = 0; i < props.nodeOutputs.size(); i++)
    {
        auto nodeOut = new NodeOutput(this);
        nodeOut->move(110, 15);
        //outAnchorPos
        if (props.nodeOutputs[i] == NODE_OUTPUT_TYPE_RGB)
        {
            nodeOut->setObjectName("RGBOut");
            this->rgbOut = nodeOut;
        }
        connect(nodeOut, &NodeOutput::nodeOutputLeftMouseClicked,
                nodeGraph, &NodeGraph::handleNodeOutputLeftMouseClicked);
    }
}

void NodeBase::setIsSelected(const bool b)
{
    isSelected = b;
    this->update();
}

void NodeBase::setIsActive(const bool b)
{
    isActive = b;
}

void NodeBase::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QRect rect = this->rect();
    painter.setBrush(defaultColorBrush);
    QPen pen(defaultColorBrush, 1);
    painter.setPen(pen);
    painter.drawRoundedRect(rect, cornerRadius, cornerRadius);

    if (isSelected)
    {
        painter.setBrush(selectedColorBrush);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(rect, cornerRadius, cornerRadius);
    }
    if (isViewed)
    {
        rect.setTopLeft(rect.topLeft() + QPoint(1, 1));
        rect.setBottomRight(rect.bottomRight() + QPoint(-1, -1));
        painter.setPen(viewedColorPen);
        painter.drawRoundedRect(rect, cornerRadius - 2, cornerRadius - 2);
    }
    Q_UNUSED(event);
}

NodeInput* NodeBase::getNodeInputAtPosition(const QPoint position)
{
    for (NodeInput* in : nodeInputs)
    {
        auto bbox = QRect(mapToGlobal(in->geometry().topLeft()),
                          mapToGlobal(in->geometry().bottomRight()));
        if (bbox.contains(position))
        {
            return in;
        }
    }
    return nullptr;
}

void NodeBase::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (!isDragging)
        {
            isDragging = true;
            oldPos = event->globalPos();
        }
        emit nodeWasLeftClicked(this);
    }
}

void NodeBase::mouseMoveEvent(QMouseEvent *event)
{
    if (isDragging)
    {
        QPoint offset = event->globalPos() - oldPos;
        QPoint newPos = this->pos() + (offset / nodeGraph->getViewScale());
        this->move(newPos);
        oldPos = event->globalPos();
        //this->updateConnectionPositions();
    }
}

void NodeBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isDragging = false;
    }
}

void NodeBase::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit nodeWasDoubleClicked(this);
    Q_UNUSED(event);
}


NodeBase::~NodeBase()
{
    delete ui;
}
