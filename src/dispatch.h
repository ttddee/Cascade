#ifndef DISPATCH_H
#define DISPATCH_H

#include <QObject>

#include "inputhandler.h"
#include "nodegraph/nodegraphview.h"

using Cascade::NodeGraph::NodeGraphView;

namespace Cascade {

class Dispatch : public QObject
{
    Q_OBJECT

public:
    explicit Dispatch(
        InputHandler* inputHandler,
        NodeGraphView* nodeGraph,
        QObject *parent = nullptr) :
        QObject(parent),
        mInputHandler(inputHandler),
        mNodeGraph(nodeGraph)
    {
        // InputHandler to NodeGraph
        connect(mInputHandler, &InputHandler::notifyFrontViewRequested,
                mNodeGraph, &NodeGraphView::handleFrontViewRequested);

        connect(mInputHandler, &InputHandler::notifyBackViewRequested,
                mNodeGraph, &NodeGraphView::handleBackViewRequested);

        connect(mInputHandler, &InputHandler::notifyAlphaViewRequested,
                mNodeGraph, &NodeGraphView::handleAlphaViewRequested);

        connect(mInputHandler, &InputHandler::notifyResultViewRequested,
                mNodeGraph, &NodeGraphView::handleResultViewRequested);
    }

private:
    InputHandler* mInputHandler;
    NodeGraphView* mNodeGraph;

};

} // namespace Cascade

#endif // DISPATCH_H
