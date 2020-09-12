#ifndef NODEIN_H
#define NODEIN_H

#include <QObject>
#include <QPushButton>

#include "nodebase.h"
#include "connection.h"

class NodeInput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeInput(QWidget *parent = nullptr);

    void addInConnection(Connection*);
    void removeInConnection();
    void updateConnection();
    bool hasConnection();

    //void triggerMouseRelease();
    NodeBase* getUpstreamNode();

    NodeBase* parentNode = nullptr;

    Connection* inConnection = nullptr;

private:


signals:
    //void mouseRealeasedOnNodeIn(NodeInput*);
    //void nodeInReceivedInConnection();
    //void nodeInLostInConnection();
};

#endif // NODEIN_H
