#ifndef NODEBASE_H
#define NODEBASE_H

#include <set>

#include <QWidget>
#include <QPen>

#include "nodedefinitions.h"
#include "nodeproperties.h"
#include "csimage.h"

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

    const NodeType nodeType;

    std::shared_ptr<CsImage> cachedImage = nullptr;;

    void setIsSelected(const bool b);
    void setIsActive(const bool b);
    void setIsViewed(const bool b);

    bool getIsViewed() const;

    NodeInput* getNodeInputAtPosition(const QPoint pos);
    NodeProperties* getProperties();
    NodeBase* getUpstreamNode();
    std::set<NodeBase*> getAllUpstreamNodes();
    void requestUpdate();
    QString getAllValues();

    bool needsUpdate = true;

private:
    void setUpNode(const NodeType nodeType);
    void createInputs(const NodeInitProperties& props);
    void createOutputs(const NodeInitProperties& props);
    void updateConnectionPositions();

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;

    void getDownstreamNodes(std::vector<NodeBase*>& nodes);
    std::vector<NodeBase*> getAllDownstreamNodes();
    void invalidateAllDownstreamNodes();

    Ui::NodeBase *ui;
    const NodeGraph* nodeGraph;
    QPoint inAnchorPos;
    QPoint outAnchorPos;
    std::vector<NodeInput*> nodeInputs;
    std::vector<NodeOutput*> nodeOutputs;
    NodeInput* rgbBackIn;
    NodeOutput* rgbOut;
    NodeProperties* propertiesView;

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
    void nodeRequestUpdate(NodeBase*);
};

#endif // NODEBASE_H
