#ifndef NODEBASE_H
#define NODEBASE_H

#include <set>

#include <QWidget>
#include <QPen>

#include "nodedefinitions.h"
#include "nodeproperties.h"
#include "csimage.h"
#include "windowmanager.h"
#include "uicolors.h"

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
    NodeBase* getUpstreamNodeBack();
    NodeBase* getUpstreamNodeFront();
    std::set<NodeBase*> getAllUpstreamNodes();
    void requestUpdate();
    QString getAllPropertyValues();
    QSize getTargetSize();
    void updateCropSizes();
    bool supportsViewerMode(const ViewerMode mode);
    bool canBeRendered();

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
    NodeInput* rgbBackIn = nullptr;
    NodeInput* rgbFrontIn = nullptr;
    NodeOutput* rgbOut = nullptr;
    NodeProperties* propertiesView;
    WindowManager* wManager;

    bool isSelected = false;
    bool isActive = false;
    bool isViewed = false;
    bool isDragging = false;
    QPoint oldPos;

    int leftCrop = 0;
    int topCrop = 0;
    int rightCrop = 0;
    int bottomCrop = 0;

    const int cornerRadius = 7;
    const QBrush defaultColorBrush = QBrush("#282d31");
    const QBrush selectedColorBrush = QBrush("#626971");
    const QPen frontViewedColorPen = QPen(frontColor, 3);
    const QPen backViewedColorPen = QPen(backColor, 3);
    const QPen alphaViewedColorPen = QPen(alphaColor, 3);
    const QPen outputViewedColorPen = QPen(outputColor, 3);

signals:
    void nodeWasLeftClicked(NodeBase*);
    void nodeWasDoubleClicked(NodeBase*);
    void nodeRequestUpdate(NodeBase*);
};

#endif // NODEBASE_H
