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

    NodeBase* parentNode = nullptr;
    const int visualWidth = 12;
    const int visualHeight = 30;

protected:
    void mousePressEvent(QMouseEvent* event) override;
//    void mouseMoveEvent(QMouseEvent* event) override;
//    void mouseReleaseEvent(QMouseEvent* event) override;

private:

    //bool isDragging = false;

signals:
    void nodeOutputLeftMouseClicked(NodeOutput* nodeOuptput);
//    void nodeOutStartsDragging(NodeBase* node);
//    void requestConnection(NodeBase* node);
//    void requestConnectionUpdate(NodeBase* node);
//    void requestConnectionAttachment(NodeBase* node);
//    void leftMouseReleaseFromNodeOut(QMouseEvent*);

};

#endif // NODEOUT_H
