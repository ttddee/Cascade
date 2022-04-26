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
#include "global.h"
#include "nodecontextmenu.h"

namespace Ui {
class NodeBase;
}

namespace Cascade {

using Renderer::CsImage;

class NodeInput;
class NodeOutput;
class NodeGraph;
class Connection;

class NodeBase : public QWidget
{
    Q_OBJECT

public:
    explicit NodeBase(
            const NodeType type,
            NodeGraph* graph,
            QWidget *parent = nullptr,
            const QString& customName = "");

    const bool operator==(const NodeBase* node) const;

    const bool isViewed() const;

    NodeInput* getNodeInputAtPosition(const QPoint pos);

    NodeProperties* getProperties() const;
    QString getAllPropertyValues() const;
    QSize getTargetSize() const;
    void addNodeToJsonArray(QJsonArray& jsonNodesArray);

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

    const NodeType getType() const;
    const bool getNeedsUpdate() const;
    void setNeedsUpdate(const bool b); // We don't want this in here

    QString getID() const;
    void setID(const QString& s);

    void setInputIDs(const QMap<int, QString>& ids);

    const std::vector<unsigned int>& getShaderCode();
    void setShaderCode(const std::vector<unsigned int> code);

    void loadNodePropertyValues(const QMap<int, QString>& values);

    NodeInput* findNodeInput(const QString& id);

    void flushCache();

    const int getNumImages();
    void switchToFirstImage();
    void switchToNextImage();

    virtual ~NodeBase();

private:
    FRIEND_TEST(NodeBaseTest, getAllDownstreamNodes_CorrectNumberOfNodes);
    FRIEND_TEST(NodeBaseTest, getAllDownstreamNodes_CorrectOrderOfNodes);
    FRIEND_TEST(NodeBaseTest, getAllUpstreamNodes_CorrectOrderOfNodes);

    // Init
    void setUpNode(const NodeType nodeType, const QString& cName = "");
    void createInputs(const NodeInitProperties& props);
    void createOutputs(const NodeInitProperties& props);

    // Graph interaction
    void getAllDownstreamNodes(std::vector<NodeBase*>& nodes);
    void updateConnectionPositions();

    // Misc
    void updateCropSizes();
    void updateRotation();

    // Events
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseDoubleClickEvent(QMouseEvent*) override;
    void paintEvent(QPaintEvent*) override;
    void moveEvent(QMoveEvent*) override;

    const NodeType mNodeType;

    std::unique_ptr<CsImage> mCachedImage;

    Ui::NodeBase *ui;
    NodeGraph* mNodeGraph;

    QString mId;

    std::vector<NodeInput*> mNodeInputs;
    std::vector<NodeOutput*> mNodeOutputs;

    NodeInput* mRgbaBackIn = nullptr;
    NodeInput* mRgbaFrontIn = nullptr;
    NodeOutput* mRgbaOut = nullptr;

    std::unique_ptr<NodeProperties> mNodeProperties;

    QString mCustomName = "";
    std::vector<unsigned int> mShaderCode;

    bool mNeedsUpdate = true;
    bool mIsSelected = false;
    bool mIsActive = false;
    bool mIsViewed = false;
    bool mIsDragging = false;

    QPoint mOldPos;

    int mLeftCrop = 0;
    int mTopCrop = 0;
    int mRightCrop = 0;
    int mBottomCrop = 0;

    int mRotation = 0;

    const QBrush mDefaultColorBrush = QBrush(Config::sDefaultNodeColor);
    const QBrush mSelectedColorBrush = QBrush(Config::sSelectedNodeColor);

signals:
    void nodeWasLeftClicked(Cascade::NodeBase* node);
    void nodeWasRightClicked(Cascade::NodeBase* node);
    void nodeWasDoubleClicked(Cascade::NodeBase* node);
    void nodeRequestUpdate(Cascade::NodeBase* node);
    void nodeRequestFileSave(
            Cascade::NodeBase* node,
            const QString& path,
            const QString& fileType,
            const QMap<std::string, std::string>& attributes,
            const bool batchRender);
    void nodeHasMoved();

public slots:
    void handleSetSelected(Cascade::NodeBase* node, const bool b);
    void handleSetActive(Cascade::NodeBase* node, const bool b);
    void handleSetViewed(Cascade::NodeBase* node, const bool b);
};

} // namespace Cascade


#endif // NODEBASE_H
