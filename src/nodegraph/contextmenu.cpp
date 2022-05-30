#include "contextmenu.h"

#include <QWidgetAction>
#include <QHeaderView>

#include "node.h"

namespace Cascade::NodeGraph {

ContextMenu::ContextMenu(
    NodeGraphScene* scene,
    QWidget* parent) :
    QMenu(parent)
{
    mScene = scene;

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
            auto& node = mScene->createNode(std::move(type));

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
