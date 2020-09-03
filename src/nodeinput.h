#ifndef NODEIN_H
#define NODEIN_H

#include <QObject>
#include <QPushButton>

#include "nodebase.h"

class NodeInput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeInput(QWidget *parent = nullptr);
    //void addInConnection(Connection*);
    void removeInConnection();
    void triggerMouseRelease();
    NodeBase* getUpstreamNode();
    //std::set<NodeBase*> getAllUpstreamNodes();
    //shared_ptr<ImageBuf> getInputImage();

    //Connection* inConnection = nullptr;
    NodeBase* parentNode = nullptr;

private:
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    //void mouseRealeasedOnNodeIn(NodeInput*);
    //void nodeInReceivedInConnection();
    //void nodeInLostInConnection();
};

#endif // NODEIN_H
