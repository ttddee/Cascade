#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QGraphicsItem>

#include "nodebase.h"

class Connection : public QObject, public QGraphicsLineItem
{
    Q_OBJECT

public:
    explicit Connection(NodeOutput* source);
//    void attachToTarget(NodeInput* target);
    QPainterPath shape() const override;
    QPoint getStartPosition();
    QPoint getEndPosition();
    void updatePosition(const QPoint end);
//    NodeBase* sourceNode = nullptr;
//    NodeInput* targetIn = nullptr;

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private:
    NodeOutput* sourceOutput = nullptr;
    NodeInput* targetInput = nullptr;

signals:
    //void requestConnectionDetachment(Connection*);
};

#endif // CONNECTION_H
