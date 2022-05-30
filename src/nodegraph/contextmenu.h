#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QTreeWidget>
#include <QAction>

#include "nodegraphscene.h"

namespace Cascade::NodeGraph {

class ContextMenu : public QMenu
{
    Q_OBJECT

public:
    ContextMenu(
        NodeGraphScene* scene,
        QWidget *parent = nullptr);

    void exec(const QPoint& position);

private:
    NodeGraphScene* mScene;
    QLineEdit* mTextBox;
    QTreeWidget* mTreeView;

    QMap<QString, QTreeWidgetItem*> mTopLevelItems;

    QPoint mScenePosition;
};

} // namespace Cascade::NodeGraph

#endif // CONTEXTMENU_H
