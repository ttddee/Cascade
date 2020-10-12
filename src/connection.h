#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>
#include <QGraphicsItem>

#include "nodebase.h"
#include "uicolors.h"

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

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*) override;

private:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem*  opt, QWidget* wdgt) override;

    const QPen normalPen = QPen(QColor("#9299a1"), 3);
    const QPen frontConnectedPen = QPen(frontColor, 3);
    const QPen backConnectedPen = QPen(backColor, 3);
    const QPen alphaConnectedPen = QPen(alphaColor, 3);

signals:
    void requestConnectionDeletion(Connection* c);

};

#endif // CONNECTION_H
