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
    QPainterPath shape() const override;
    QPoint getStartPosition();
    QPoint getEndPosition();

    void updatePosition();
    void updatePosition(const QPoint end);

    NodeInput* targetInput = nullptr;
    NodeOutput* sourceOutput = nullptr;

private:



};

#endif // CONNECTION_H
