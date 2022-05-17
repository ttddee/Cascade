#include "nodegraphmodel.h"

#include <QJsonArray>
#include <QJsonObject>

#include "nodefactory.h"
#include "connection.h"
#include "nodeinput.h"
#include "nodeoutput.h"
#include "../log.h"

namespace Cascade {

NodeGraphModel::NodeGraphModel(QObject *parent)
    : QObject(parent)
{

}

NodeBase* NodeGraphModel::getViewedNode()
{
    return mViewedNode;
}

NodeBase* NodeGraphModel::getSelectedNode()
{
    return mSelectedNode;
}

void NodeGraphModel::createProject()
{
    addNode(NodeType::eRead, NodeGraphPosition::eCustom, "", QPoint(29600, 29920), false);
    addNode(NodeType::eWrite, NodeGraphPosition::eCustom, "", QPoint(30200, 29920), false);
}

void NodeGraphModel::addNode(
        const NodeType type,
        const NodeGraphPosition position,
        const QString& customName,
        const QPoint coords,
        const bool view)
{
    auto* n = NodeFactory::createNode(type, nullptr, customName);
    //mScene->addWidget(n);

    QPoint nodePos;

    if (position == NodeGraphPosition::eCustom)
        nodePos = coords;
//    else
//        nodePos = getCoordinatesForPosition(position);

    n->move(nodePos);

    mNodes.push_back(n);

    connectNodeSignals(n);

    // Connect node if necessary
    if (mSelectedNode && n->getType() != NodeType::eRead)
    {
        createOpenConnection(mSelectedNode->getRgbaOut());
        establishConnection(n->getOpenInput());
    }

    if (view)
        viewNode(n);

    mLastCreatedNodePos = nodePos;

    emit nodeGraphIsDirty();
}

void NodeGraphModel::deleteNode(NodeBase *node)
{
    node->invalidateAllDownstreamNodes();

    auto connections = node->getAllConnections();
    foreach (auto& c, connections)
    {
        deleteConnection(c);
    }

    mNodes.erase(remove(mNodes.begin(), mNodes.end(), node), mNodes.end());

    //mScene->removeItem(node->graphicsProxyWidget());

    if (node == mViewedNode)
    {
        emit viewedNodeHasBeenDeleted();
        mViewedNode = nullptr;
    }
    if (node == mActiveNode)
    {
        emit activeNodeHasBeenDeleted();
        mActiveNode = nullptr;
    }

    delete node;

    mSelectedNode = nullptr;

    emit nodeGraphIsDirty();
}

void NodeGraphModel::selectNode(NodeBase *node)
{
    mSelectedNode = node;
    foreach(auto* n, mNodes)
    {
        //emit requestSetNodeSelected(n, false);
    }
    //emit requestSetNodeSelected(node, true);
}

void NodeGraphModel::activateNode(NodeBase *node)
{
    mActiveNode = node;
    //emit requestSetNodeActive(node, true);
}

void NodeGraphModel::viewNode(NodeBase *node)
{
    if (node)
    {
        mViewedNode = node;

        foreach(NodeBase* n, mNodes)
        {
            //emit requestSetNodeViewed(n, false);
        }
        //emit requestSetNodeViewed(node, true);
        //emit requestNodeDisplay(node);
    }
}

NodeBase* NodeGraphModel::findNodeById(const QString& id)
{
    foreach(NodeBase* n, mNodes)
    {
        if (n->getID() == id)
            return n;
    }
    return nullptr;
}

Connection* NodeGraphModel::createOpenConnection(NodeOutput* nodeOut)
{
    Connection* c = new Connection(nodeOut);
    //mScene->addItem(c);
    mOpenConnection = c;

//    mOpenConnection->updatePosition(
//                QPoint(mapToScene(mapFromGlobal(QCursor::pos())).x(),
//                       mapToScene(mapFromGlobal(QCursor::pos())).y()));

    return c;
}

void NodeGraphModel::destroyOpenConnection()
{
    if(mOpenConnection)
    {
        //mScene->removeItem(mOpenConnection);
        delete mOpenConnection;
        mOpenConnection = nullptr;
    }
}

void NodeGraphModel::establishConnection(NodeInput *nodeIn)
{
    mOpenConnection->connectToTarget(nodeIn);
    mOpenConnection->getSourceOutput()->addConnection(mOpenConnection);
    mConnections.push_back(mOpenConnection);
    nodeIn->addInConnection(mOpenConnection);
    mOpenConnection = nullptr;

    emit nodeGraphIsDirty();
}

void NodeGraphModel::loadConnection(NodeOutput* src, NodeInput* dst)
{
    Connection* c = new Connection(src);
    //mScene->addItem(c);
    c->connectToTarget(dst);
    c->getSourceOutput()->addConnection(c);
    mConnections.push_back(c);
    dst->addInConnectionNoUpdate(c);
}

void NodeGraphModel::deleteConnection(Connection* c)
{
    for(size_t i = 0; i < mConnections.size(); ++i)
    {
        if (c == mConnections[i])
        {
            mConnections.erase(mConnections.begin() + i);
        }
    }

    c->getSourceOutput()->removeConnection(c);
    c->getTargetInput()->removeInConnection();

    //mScene->removeItem(c);
    delete c;
    c = nullptr;

    emit nodeGraphIsDirty();
}

NodeBase* NodeGraphModel::loadNode(const NodePersistentProperties& p)
{
    NodeBase* n = new NodeBase(p.nodeType, nullptr, p.customName);
    //mScene->addWidget(n);
    n->move(p.pos);
    mNodes.push_back(n);

    connectNodeSignals(n);

    n->loadNodePropertyValues(p);

    return n;
}

void NodeGraphModel::connectNodeSignals(NodeBase* n)
{
    // Node to graph
//    connect(n, &NodeBase::nodeWasLeftClicked,
//                this, &NodeGraph::handleNodeLeftClicked);
//    connect(n, &NodeBase::nodeWasDoubleClicked,
//            this, &NodeGraph::handleNodeDoubleClicked);
//    connect(n, &NodeBase::nodeRequestUpdate,
//            this, &NodeGraph::handleNodeUpdateRequest);
//    // Graph to node
//    connect(this, &NodeGraph::requestSetNodeSelected,
//            n, &NodeBase::handleSetSelected);
//    connect(this, &NodeGraph::requestSetNodeActive,
//            n, &NodeBase::handleSetActive);
//    connect(this, &NodeGraph::requestSetNodeViewed,
//            n, &NodeBase::handleSetViewed);

//    connect(n, &NodeBase::nodeWasDoubleClicked,
//            mWindowManager, &WindowManager::handleNodeDoubleClicked);

    if (n->getType() == NodeType::eWrite)
    {
//        connect(n, &NodeBase::nodeRequestFileSave,
//                this, &NodeGraph::handleFileSaveRequest);
    }
}

void NodeGraphModel::loadProject(const QJsonArray& jsonNodeGraph)
{
    QJsonObject jsonNodesHeading = jsonNodeGraph.at(0).toObject();
    QJsonArray jsonNodesArray = jsonNodesHeading.value("nodes").toArray();
    QJsonObject jsonConnectionsHeading = jsonNodeGraph.at(1).toObject();
    QJsonArray jsonConnectionsArray = jsonConnectionsHeading.value("connections").toArray();

    clearGraph();

    for (int i = 0; i < jsonNodesArray.size(); i++)
    {
        QJsonObject jsonNode = jsonNodesArray.at(i).toObject();

        // Set properties to loaded values
        NodePersistentProperties p;
        p.nodeType = nodeStrings.key(jsonNode["type"].toString());
        p.pos = QPoint(jsonNode["posx"].toInt(), jsonNode["posy"].toInt());
        p.uuid = jsonNode["uuid"].toString();
        p.customName = jsonNode["customname"].toString();

        // Get UUID for Node Inputs
        QJsonObject ins = jsonNode["inputs"].toObject();
        for (int i = 0; i < ins.size(); i++)
        {
            p.inputs[i] = ins.value(QString::number(i)).toString();
        }

        // Load properties to array
        QJsonObject props = jsonNode["properties"].toObject();
        for (int i = 0; i < props.size(); i++)
        {
            p.properties[i] = props.value(QString::number(i)).toString();
        }

        loadNode(p);
    }

    for (int i = 0; i < jsonConnectionsArray.size(); i++)
    {
        auto src = jsonConnectionsArray.at(i)["src"].toString();
        auto dst = jsonConnectionsArray.at(i)["dst"].toString();
        auto dstNode = jsonConnectionsArray.at(i)["dst-node"].toString();

        NodeBase* srcNode = nullptr;
        NodeInput* targetInput = nullptr;
        bool found = false;
        if (NodeBase* n = findNodeById(src))
        {
            srcNode = n;
            if (NodeBase* n = findNodeById(dstNode))
            {

                if (NodeInput* in = n->findNodeInput(dst))
                {
                    targetInput = in;
                    loadConnection(srcNode->getRgbaOut(), targetInput);
                    found = true;
                }
            }
        }
        if (!found)
            CS_LOG_WARNING("Could not load connection.");
    }
}

void NodeGraphModel::getNodeGraphAsJson(QJsonArray& jsonNodeGraph)
{
    QJsonArray jsonNodesArray;

    foreach (const auto& node, mNodes)
    {
        node->addNodeToJsonArray(jsonNodesArray);
    }
    QJsonObject jsonNodesHeading {
        { "nodes", jsonNodesArray }
    };
    jsonNodeGraph.push_back(jsonNodesHeading);

    QJsonArray jsonConnectionsArray;
    foreach (const auto& connection, mConnections)
    {
        connection->addConnectionToJsonObject(jsonConnectionsArray);
    }
    QJsonObject jsonConnectionsHeading {
        { "connections", jsonConnectionsArray }
    };
    jsonNodeGraph.push_back(jsonConnectionsHeading);
}

void NodeGraphModel::clearGraph()
{
    foreach (const auto& connection, mConnections)
    {
        deleteConnection(connection);
    }
    foreach (const auto& node, mNodes)
    {
        deleteNode(node);
    }
}

void NodeGraphModel::flushCacheAllNodes()
{
    for (auto& n : mNodes)
        n->flushCache();
}

} // namespace Cascade
