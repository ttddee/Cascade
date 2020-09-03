#ifndef NODEBASE_H
#define NODEBASE_H

#include <QWidget>
#include <QPen>

#include "nodedefinitions.h"

using namespace Cascade;

class NodeInput;
class NodeOutput;
class NodeGraph;

namespace Ui {
class NodeBase;
}

class NodeBase : public QWidget
{
    Q_OBJECT

public:
    explicit NodeBase(const NodeType type, const NodeGraph* graph, QWidget *parent = nullptr);
    ~NodeBase();

    void setIsSelected(const bool b);
    void setIsActive(const bool b);

    NodeInput* getNodeInputAtPosition(const QPoint pos);

private:
    void setUpNode(const NodeType nodeType);
    void createInputs(const NodeInitProperties& props);
    void createOutputs(const NodeInitProperties& props);

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;

    Ui::NodeBase *ui;
    const NodeType nodeType;
    const NodeGraph* nodeGraph;
    QPoint inAnchorPos;
    QPoint outAnchorPos;
    std::vector<NodeInput*> nodeInputs;
    std::vector<NodeOutput*> nodeOutputs;
    NodeInput* rgbBackIn;
    NodeOutput* rgbOut;

    bool isSelected = false;
    bool isActive = false;
    bool isViewed = false;
    bool isDragging = false;
    QPoint oldPos;

    const int cornerRadius = 7;
    const QBrush defaultColorBrush = QBrush("#282d31");
    const QBrush selectedColorBrush = QBrush("#626971");
    const QPen viewedColorPen = QPen(QColor("#dc4c46"), 3);

signals:
    void nodeWasLeftClicked(NodeBase*);
    void nodeWasDoubleClicked(NodeBase*);
};

#endif // NODEBASE_H
