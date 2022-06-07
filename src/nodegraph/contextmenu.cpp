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

#include "contextmenu.h"

#include <QWidgetAction>
#include <QHeaderView>

#include "node.h"

namespace Cascade::NodeGraph {

ContextMenu::ContextMenu(
    NodeGraphDataModel* model,
    NodeGraphScene* scene,
    QWidget* parent) :
    QMenu(parent)
{
    mScene = scene;
    mModel = model;

    //Add filterbox to the context menu
    mTextBox = new QLineEdit(this);

    mTextBox->setPlaceholderText(QStringLiteral("Filter"));
    mTextBox->setClearButtonEnabled(true);

    auto *txtBoxAction = new QWidgetAction(this);
    txtBoxAction->setDefaultWidget(mTextBox);

    addAction(txtBoxAction);

    //Add result treeview to the context menu
    mTreeView = new QTreeWidget(this);
    mTreeView->header()->close();

    auto *treeViewAction = new QWidgetAction(this);
    treeViewAction->setDefaultWidget(mTreeView);

    addAction(treeViewAction);

    for (auto const &cat : mScene->registry().categories())
    {
        auto item = new QTreeWidgetItem(mTreeView);
        item->setText(0, cat);
        mTopLevelItems[cat] = item;
    }

    for (auto const &assoc : mScene->registry().registeredModelsCategoryAssociation())
    {
        auto parent = mTopLevelItems[assoc.second];
        auto item   = new QTreeWidgetItem(parent);
        item->setText(0, assoc.first);
        item->setData(0, Qt::UserRole, assoc.first);
    }

    mTreeView->expandAll();

    connect(mTreeView, &QTreeWidget::itemClicked, [&](QTreeWidgetItem *item, int column)
    {
        QString modelName = item->data(0, Qt::UserRole).toString();

        auto type = mScene->registry().create(modelName);

        if (type)
        {
            auto& node = mModel->createNode(std::move(type));

            QPoint posView = mScenePosition;

            node.nodeGraphicsObject().setPos(posView);

            emit mScene->nodePlaced(node);
        }
        else
        {
            // qDebug() << "Model not found";
        }

        close();
    });

    //Setup filtering
    connect(mTextBox, &QLineEdit::textChanged, [&](const QString &text)
    {
        for (auto& topLvlItem : mTopLevelItems)
        {
            for (int i = 0; i < topLvlItem->childCount(); ++i)
            {
                auto child = topLvlItem->child(i);
                auto modelName = child->data(0, Qt::UserRole).toString();
                const bool match = (modelName.contains(text, Qt::CaseInsensitive));
                child->setHidden(!match);
            }
        }
    });
}

void ContextMenu::exec(const QPoint& position)
{
    // make sure the text box gets focus so the user doesn't have to click on it
    mTextBox->setFocus();
    mScenePosition = position;

    QMenu::exec(QCursor::pos());
}

} // namespace Cascade::NodeGraph
