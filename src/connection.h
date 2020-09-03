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

    void updatePosition();
    void updatePosition(const QPoint end);

    NodeInput* targetInput = nullptr;
    NodeOutput* sourceOutput = nullptr;

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*  opt, QWidget* wdgt) override;

    const QPen normalPen = QPen(QColor("#9299a1"), 3);
    const QPen connectedPen = QPen(QColor("#9bcf43"), 3);

};

#endif // CONNECTION_H
