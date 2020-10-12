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
    explicit NodeInput(NodeInputType t, QWidget *parent = nullptr);

    void addInConnection(Connection*);
    void removeInConnection();
    void updateConnection();
    bool hasConnection();

    Connection* getConnection();

    NodeInputType getInputType();

    NodeBase* getUpstreamNode();

    NodeBase* parentNode = nullptr;

    Connection* inConnection = nullptr;

private:
    const NodeInputType inputType;


signals:

};

#endif // NODEIN_H
