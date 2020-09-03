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
    void triggerMouseRelease();
    NodeBase* getUpstreamNode();
    bool hasConnection();
    void updateConnection();
    //std::set<NodeBase*> getAllUpstreamNodes();
    //shared_ptr<ImageBuf> getInputImage();

    NodeBase* parentNode = nullptr;

private:
    Connection* inConnection = nullptr;

signals:
    //void mouseRealeasedOnNodeIn(NodeInput*);
    //void nodeInReceivedInConnection();
    //void nodeInLostInConnection();
};

#endif // NODEIN_H
