#include "nodeoutput.h"
#include <QMouseEvent>
#include <iostream>
#include <QGraphicsLineItem>
#include <QPen>
#include <QColor>

NodeOutput::NodeOutput(QWidget *parent)
    : QPushButton (parent)
{
    // Keep reference to parent node
    parentNode = static_cast<NodeBase*>(parent);

    // Register connections to Controller
    //Controller* controller;
    //controller = &Controller::getInstance();
//    connect(this, &NodeOut::requestConnection, controller, &Controller::handleConnectionRequest);
//    connect(this, &NodeOut::requestConnectionUpdate, controller, &Controller::handleConnectionUpdateRequest);
//    //connect(this, &NodeOut::requestConnectionAttachment, controller, &Controller::handleConnectionAttachmentRequest);

    this->resize(visualWidth, visualHeight);
}

void NodeOutput::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit nodeOutputLeftMouseClicked(this);
        //this->isDragging = true;
    }
}

//void NodeOutput::mouseMoveEvent(QMouseEvent *event)
//{
//    if (isDragging)
//    {
//        emit requestConnectionUpdate(parentNode);
//    }
//    Q_UNUSED(event);
//}

//void NodeOutput::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (isDragging)
//    {
//        isDragging = false;
//        emit leftMouseReleaseFromNodeOut(event);
//        //emit requestConnectionAttachment(parentNode);
//    }
//}
