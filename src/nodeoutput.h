#ifndef NODEOUT_H
#define NODEOUT_H

#include <QObject>
#include <QPushButton>
#include <QGraphicsItem>

#include "nodebase.h"
#include "connection.h"

class NodeOutput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeOutput(QWidget *parent = nullptr);

    void addConnection(Connection* c);
    void removeConnection(Connection* c);
    void updateConnections();
    std::vector<Connection*> getConnections();

    NodeBase* parentNode = nullptr;
    const int visualWidth = 12;
    const int visualHeight = 25;

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    std::vector<Connection*> outConnections;

signals:
    void nodeOutputLeftMouseClicked(NodeOutput* nodeOuptput);

};

#endif // NODEOUT_H
