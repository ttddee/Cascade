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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QTreeWidget>
#include <QAction>

#include "nodegraphscene.h"
#include "nodegraphdatamodel.h"

namespace Cascade::NodeGraph {

class ContextMenu : public QMenu
{
    Q_OBJECT

public:
    ContextMenu(
        NodeGraphDataModel* model,
        NodeGraphScene* scene,
        QWidget *parent = nullptr);

    void exec(const QPoint& position);

private:
    NodeGraphDataModel* mModel;
    NodeGraphScene* mScene;
    QLineEdit* mTextBox;
    QTreeWidget* mTreeView;

    QMap<QString, QTreeWidgetItem*> mTopLevelItems;

    QPoint mScenePosition;
};

} // namespace Cascade::NodeGraph

#endif // CONTEXTMENU_H
