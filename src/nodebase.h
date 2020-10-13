/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
class Connection;

namespace Ui {
class NodeBase;
}

enum DisplayMode
{
    DISPLAY_MODE_RGB,
    DISPLAY_MODE_ALPHA
};

class NodeBase : public QWidget
{
    Q_OBJECT

public:
    explicit NodeBase(const NodeType type, const NodeGraph* graph, QWidget *parent = nullptr);

    const NodeType nodeType;

    std::shared_ptr<CsImage> cachedImage = nullptr;;

    void setIsSelected(const bool b);
    void setIsActive(const bool b);
    void setIsViewed(const bool b);

    bool getIsViewed() const;

    NodeInput* getNodeInputAtPosition(const QPoint pos);

    NodeProperties* getProperties();
    QString getAllPropertyValues();
    QSize getTargetSize();

    NodeBase* getUpstreamNodeBack();
    NodeBase* getUpstreamNodeFront();

    std::set<NodeBase*> getAllUpstreamNodes();
    std::set<Connection*> getAllConnections();

    void invalidateAllDownstreamNodes();

    bool canBeRendered();

    void requestUpdate();

    virtual ~NodeBase();

    bool needsUpdate = true;

private:
    void setUpNode(const NodeType nodeType);
    void createInputs(const NodeInitProperties& props);
    void createOutputs(const NodeInitProperties& props);

    void updateConnectionPositions();
    void getDownstreamNodes(std::vector<NodeBase*>& nodes);
    std::vector<NodeBase*> getAllDownstreamNodes();

    void updateCropSizes();
    void updateRotation();
    void updateResizeFactor();

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;

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

    int rotation = 0;

    float resizeFactor = 1.0;

    const int cornerRadius = 7;
    const QBrush defaultColorBrush = QBrush("#282d31");
    const QBrush selectedColorBrush = QBrush("#626971");
    const QPen frontViewedColorPen = QPen(frontColor, 3);
    const QPen backViewedColorPen = QPen(backColor, 3);
    const QPen alphaViewedColorPen = QPen(alphaColor, 3);
    const QPen outputViewedColorPen = QPen(outputColor, 3);

signals:
    void nodeWasLeftClicked(NodeBase* node);
    void nodeWasDoubleClicked(NodeBase* node);
    void nodeRequestUpdate(NodeBase* node);
    void nodeRequestFileSave(NodeBase* node, const QString& path);
};

#endif // NODEBASE_H
