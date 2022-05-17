#ifndef NODEGRAPHMODEL_H
#define NODEGRAPHMODEL_H

#include <QObject>

#include "nodebase.h"

namespace Cascade {

class NodeGraphModel : public QObject
{
    Q_OBJECT

public:
    explicit NodeGraphModel(QObject *parent = nullptr);

    NodeBase* getViewedNode();
    NodeBase* getSelectedNode();

    void getNodeGraphAsJson(QJsonArray& jsonNodeGraph);

private:
    // Project
    void createProject();
    void loadProject(const QJsonArray& jsonNodeGraph);
    void clearGraph();

    // Nodes
    void addNode(
            const NodeType type,
            const NodeGraphPosition position,
            const QString& customName = "",
            const QPoint coords = QPoint(0, 0),
            const bool view = true);
    void viewNode(NodeBase* node);
    void deleteNode(NodeBase* node);
    void selectNode(NodeBase* node);
    void activateNode(NodeBase* node);
    NodeBase* findNodeById(const QString& id);
    NodeBase* loadNode(const NodePersistentProperties& p);
    void connectNodeSignals(NodeBase* n);
    void flushCacheAllNodes();

    // Connections
    Connection* createOpenConnection(NodeOutput* nodeOut);
    void establishConnection(NodeInput* nodeIn);
    void destroyOpenConnection();
    void deleteConnection(Connection* c);
    void loadConnection(NodeOutput* src, NodeInput* dst);

    std::vector<NodeBase*> mNodes;
    std::vector<Connection*> mConnections;
    Connection* mOpenConnection = nullptr;

    // The selected node
    NodeBase* mSelectedNode = nullptr;
    // The node with active properties
    NodeBase* mActiveNode = nullptr;
    // The node that is being displayed
    NodeBase* mViewedNode = nullptr;

    QPoint mLastCreatedNodePos = QPoint(29700, 29920);

signals:
    void nodeGraphIsDirty();
    void viewedNodeHasBeenDeleted();
    void activeNodeHasBeenDeleted();
};

} // namespace Cascade

#endif // NODEGRAPHMODEL_H
