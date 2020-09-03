#include "nodeinput.h"

#include <iostream>

#include <QMouseEvent>

NodeInput::NodeInput(QWidget *parent)
    : QPushButton (parent)
{
    // Keep reference to parent node
    parentNode = static_cast<NodeBase*>(parent);
    this->resize(12, 30);

}

void NodeInput::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        std::cout << "Release" << std::endl;
        //this->isDragging = true;
    }
}

//void NodeInput::addInConnection(Connection *c)
//{
//    inConnection = c;
//    emit nodeInReceivedInConnection();
//}

//void NodeInput::removeInConnection()
//{
//    inConnection = nullptr;
//    emit nodeInLostInConnection();
//}

//void NodeInput::triggerMouseRelease()
//{
//    emit mouseRealeasedOnNodeIn(this);
//}

//NodeBase* NodeInput::getUpstreamNode()
//{
//    return inConnection ? inConnection->sourceNode : nullptr;
//}

//std::set<NodeBase*> NodeInput::getAllUpstreamNodes()
//{
//    std::set<NodeBase*> nodes;
//    if (auto n = getUpstreamNode())
//    {
//        //n->getUpstreamNode();
//        nodes = n->getAllUpstreamNodes();
//    }
//    nodes.insert(this->parentNode);
//    return nodes;
//}

//shared_ptr<ImageBuf> NodeIn::getInputImage()
//{
//    if (auto n = getUpstreamNode())
//        return n->getOutputImage();
//    else
//        return shared_ptr<ImageBuf>(new ImageBuf);
//}
