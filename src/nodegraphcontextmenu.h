#ifndef NODEGRAPHCONTEXTMENU_H
#define NODEGRAPHCONTEXTMENU_H

#include <QMenu>

class NodeGraph;

class NodeGraphContextMenu : public QMenu
{
public:
    NodeGraphContextMenu(NodeGraph* parent);

private:
    std::vector<QAction*> actions;
};

#endif // NODEGRAPHCONTEXTMENU_H
