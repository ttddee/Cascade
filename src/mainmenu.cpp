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

#include "mainmenu.h"

#include "mainwindow.h"
//#include "nodegraph/nodedefinitions.h"

namespace Cascade {

MainMenu::MainMenu(MainWindow* mainWindow)
{
    // Note:
    // It is OK for the QActions to have their menu as a parent.
    // They won't be used anywhere else.
    // Otherwise we would have to delete them manually.

    // File Menu
    mFileMenu = new QMenu("File");
    this->addMenu(mFileMenu);

    mNewProjectAction = new QAction("New Project" , mFileMenu);
    mNewProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    mFileMenu->addAction(mNewProjectAction);
    connect(mNewProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleNewProjectAction);

    mFileMenu->addSeparator();

    mOpenProjectAction = new QAction("Open Project" , mFileMenu);
    mOpenProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    mFileMenu->addAction(mOpenProjectAction);
    connect(mOpenProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleOpenProjectAction);

    mSaveProjectAction = new QAction("Save Project" , mFileMenu);
    mSaveProjectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    mFileMenu->addAction(mSaveProjectAction);
    connect(mSaveProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleSaveProjectAction);

    mSaveProjectAsAction = new QAction("Save Project As..." , mFileMenu);
    mSaveProjectAsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    mFileMenu->addAction(mSaveProjectAsAction);
    connect(mSaveProjectAsAction, &QAction::triggered,
            mainWindow, &MainWindow::handleSaveProjectAsAction);

    mFileMenu->addSeparator();

    mExitAction = new QAction("Exit" , mFileMenu);
    mFileMenu->addAction(mExitAction);
    connect(mExitAction, &QAction::triggered,
            mainWindow, &MainWindow::handleExitAction,
            Qt::QueuedConnection);

    // Edit Menu
    mEditMenu = new QMenu("Edit");
    this->addMenu(mEditMenu);

    auto createNodeMenu = mEditMenu->addMenu("Create Node");

    // TODO: This is doubled in nodegraphcontextmenu

    // Populate menu with submenus aka categories
//    QMap<NodeCategory, QMenu*> categories;

//    {
//        QMapIterator<NodeCategory, QString> i(categoryStrings);
//        while (i.hasNext())
//        {
//            i.next();
//            auto submenu = createNodeMenu->addMenu(categoryStrings[i.key()]);
//            categories[i.key()] = submenu;
//        }
//    }

//    // Add nodes to corresponding submenus
//    auto nodes = nodeStrings;
//    nodes.remove(NodeType::eIsf);
//    QMapIterator<NodeType, QString> i(nodes);
//    while (i.hasNext())
//    {
//        i.next();
//        auto a = new QAction();
//        mCreateNodeActions.push_back(a);
//        a->setText(i.value());
//        auto t = i.key();
//        categories[getPropertiesForType(t).category]->addAction(a);

//        QObject::connect(
//                    a,
//                    &QAction::triggered,
//                    this,
//                    [this, t]{ emit requestNodeCreation(
//                        t, NodeGraphPosition::eRelativeToLastNode); });
//    }

    // Add ISF categories
//    auto isfManager = &ISFManager::getInstance();
//    std::set<QString> isfCategoryStrings = isfManager->getCategories();

//    QMap<QString, QMenu*> isfCategories;
//    {
//        for (auto& cat : isfCategoryStrings)
//        {
//            auto submenu = categories.value(NodeCategory::eIsf)->addMenu(cat);
//            isfCategories[cat] = submenu;
//        }
//    }

    // Add ISF nodes
//    auto isfNodeProperties = isfManager->getNodeProperties();
//    for (auto& prop : isfNodeProperties)
//    {
//        QString nodeName = prop.second.title;
//        auto a = new QAction();
//        mCreateNodeActions.push_back(a);
//        a->setText(nodeName);
//        auto t = NodeType::eIsf;
//        isfCategories[isfManager->getCategoryPerNode(nodeName)]->addAction(a);

//        QObject::connect(
//                    a,
//                    &QAction::triggered,
//                    this,
//                    [this, t, nodeName]{ emit requestNodeCreation(
//                        t,
//                        NodeGraphPosition::eRelativeToLastNode,
//                        nodeName); });
//    }
//    auto graph = mainWindow->getNodeGraph();
//    connect(this, &MainMenu::requestNodeCreation,
//            graph, &NodeGraph::handleNodeCreationRequest);

//    mEditMenu->addSeparator();

//    mPreferencesAction = new QAction("Preferences..." , mEditMenu);
//    mEditMenu->addAction(mPreferencesAction);
//    connect(mPreferencesAction, &QAction::triggered,
//            mainWindow, &MainWindow::handlePreferencesAction);

    // View Menu
    mViewMenu = new QMenu("View");
    this->addMenu(mViewMenu);

    mViewMenu->addAction(mainWindow->mNodeGraphDockWidget->toggleViewAction());
    mViewMenu->addAction(mainWindow->mPropertiesViewDockWidget->toggleViewAction());

    mViewMenu->addSeparator();

    // Help Menu
    mHelpMenu = new QMenu("Help");
    this->addMenu(mHelpMenu);

    mAboutAction = new QAction("About", mHelpMenu);
    mHelpMenu->addAction(mAboutAction);
    connect(mAboutAction, &QAction::triggered,
            mainWindow, &MainWindow::handleAboutAction);
}

MainMenu::~MainMenu()
{
    foreach (auto& action, mCreateNodeActions)
    {
        delete action;
    }
}

} // namespace Cascade
