/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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
#include <memory>

#include <QPen>

#include <gtest/gtest_prod.h>

#include "nodedefinitions.h"
#include "nodeproperties.h"
#include "renderer/csimage.h"
#include "windowmanager.h"
#include "uicolors.h"

namespace Ui {
class NodeBase;
}

namespace Cascade {

using Renderer::CsImage;

class NodeInput;
class NodeOutput;
class NodeGraph;
class Connection;

enum DisplayMode
{
    DISPLAY_MODE_RGB,
    DISPLAY_MODE_ALPHA
};

class NodeBase : public QWidget
{
    Q_OBJECT

public:
    explicit NodeBase(
            const NodeType type,
            const NodeGraph* graph,
            QWidget *parent = nullptr,
            const QString& customName = "");

    const NodeType nodeType;

    void setIsSelected(const bool b);
    void setIsActive(const bool b);
    void setIsViewed(const bool b);

    bool getIsViewed() const;

    NodeInput* getNodeInputAtPosition(const QPoint pos);

    NodeProperties* getProperties() const;
    QString getAllPropertyValues() const;
    QSize getTargetSize() const;
    void addNodeToJsonArray(QJsonArray& jsonNodesArray);

    NodeInput* getRgbaBackIn() const;
    NodeInput* getRgbaFrontIn() const;
    NodeOutput* getRgbaOut() const;

    NodeBase* getUpstreamNodeBack() const;
    NodeBase* getUpstreamNodeFront()const;

    void getAllUpstreamNodes(std::vector<NodeBase*>& nodes);
    std::set<Connection*> getAllConnections();

    CsImage* getCachedImage() const;
    void setCachedImage(std::unique_ptr<CsImage> image);

    void invalidateAllDownstreamNodes();

    bool canBeRendered() const;

    void requestUpdate();

    NodeInput* getOpenInput() const;
    QSize getInputSize() const;

    QString getID() const;
    void setID(const QString& s);

    void setInputIDs(const QMap<int, QString>& ids);

    const std::vector<unsigned int>& getShaderCode();
    void setShaderCode(const std::vector<unsigned int> code);

    void loadNodePropertyValues(const QMap<int, QString>& values);

    NodeInput* findNodeInput(const QString& id);

    void updateConnectionPositions();

    void flushCache();

    const int getNumImages();
    void switchToFirstImage();
    void switchToNextImage();

    virtual ~NodeBase();

    bool needsUpdate = true;

private:
    FRIEND_TEST(NodeBaseTest, getAllDownstreamNodes_CorrectNumberOfNodes);
    FRIEND_TEST(NodeBaseTest, getAllDownstreamNodes_CorrectOrderOfNodes);
    FRIEND_TEST(NodeBaseTest, getAllUpstreamNodes_CorrectOrderOfNodes);

    void setUpNode(const NodeType nodeType, const QString& cName = "");
    void createInputs(const NodeInitProperties& props);
    void createOutputs(const NodeInitProperties& props);

    void getAllDownstreamNodes(std::vector<NodeBase*>& nodes);

    void updateCropSizes();
    void updateRotation();

    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void moveEvent(QMoveEvent*) override;

    std::unique_ptr<CsImage> cachedImage;

    Ui::NodeBase *ui;
    const NodeGraph* nodeGraph;

    QString id;

    QPoint inAnchorPos;
    QPoint outAnchorPos;

    std::vector<NodeInput*> nodeInputs;
    std::vector<NodeOutput*> nodeOutputs;

    NodeInput* rgbaBackIn = nullptr;
    NodeInput* rgbaFrontIn = nullptr;
    NodeOutput* rgbaOut = nullptr;

    std::unique_ptr<NodeProperties> nodeProperties;

    WindowManager* wManager;

    QString customName = "";
    std::vector<unsigned int> shaderCode;

    bool isSelected = false;
    bool isActive = false;
    bool isViewed = false;
    bool isDragging = false;

    QPoint oldPos;

    bool hasCustomSize = false;
    UiEntity* sizeSource;

    int leftCrop = 0;
    int topCrop = 0;
    int rightCrop = 0;
    int bottomCrop = 0;

    int rotation = 0;

    const int cornerRadius = 6;
    const QBrush defaultColorBrush = QBrush(QColor(24, 27, 30));
    const QBrush selectedColorBrush = QBrush(QColor(37, 74, 115));
    const QPen defaultColorPen = QPen(QColor(0x62, 0x69, 0x71), 3);

signals:
    void nodeWasLeftClicked(Cascade::NodeBase* node);
    void nodeWasDoubleClicked(Cascade::NodeBase* node);
    void nodeRequestUpdate(Cascade::NodeBase* node);
    void nodeRequestFileSave(
            Cascade::NodeBase* node,
            const QString& path,
            const QString& fileType,
            const QMap<std::string, std::string>& attributes,
            const bool batchRender);
    void nodeHasMoved();
};

} // namespace Cascade


#endif // NODEBASE_H
