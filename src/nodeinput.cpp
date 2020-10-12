#include "nodeinput.h"

#include <iostream>

#include <QMouseEvent>

NodeInput::NodeInput(NodeInputType t, QWidget *parent)
    : QPushButton (parent),
      inputType(t)
{
    parentNode = static_cast<NodeBase*>(parent);
    this->resize(12, 25);

    if (t == NODE_INPUT_TYPE_RGB_FRONT)
        setObjectName("Front");
    if (t == NODE_INPUT_TYPE_RGB_BACK)
        setObjectName("Back");
    if (t == NODE_INPUT_TYPE_ALPHA)
        setObjectName("Alpha");
}

NodeInputType NodeInput::getInputType()
{
    return inputType;
}

bool NodeInput::hasConnection()
{
    if(inConnection)
    {
        return true;
    }
    return false;
}

Connection* NodeInput::getConnection()
{
    if (hasConnection())
        return inConnection;

    return nullptr;
}

void NodeInput::updateConnection()
{
    if(inConnection)
    {
        inConnection->updatePosition();
    }
}

void NodeInput::addInConnection(Connection* c)
{
    inConnection = c;
    updateConnection();
    parentNode->requestUpdate();
}

void NodeInput::removeInConnection()
{
    inConnection = nullptr;
    parentNode->requestUpdate();
}

