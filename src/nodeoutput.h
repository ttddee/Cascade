#ifndef NODEOUT_H
#define NODEOUT_H

#include <QObject>
#include <QPushButton>
#include <QGraphicsItem>
#include "nodebase.h"

class NodeOutput : public QPushButton
{
    Q_OBJECT

public:
    explicit NodeOutput(QWidget *parent = nullptr);

protected:
//    void mousePressEvent(QMouseEvent* event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    NodeBase* parentNode = nullptr;
    bool isDragging = false;

signals:
    void nodeOutStartsDragging(NodeBase* node);
    void requestConnection(NodeBase* node);
    void requestConnectionUpdate(NodeBase* node);
    void requestConnectionAttachment(NodeBase* node);
    void leftMouseReleaseFromNodeOut(QMouseEvent*);

};

#endif // NODEOUT_H
