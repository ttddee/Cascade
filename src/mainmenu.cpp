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
#include "nodedefinitions.h"

using namespace Cascade;

MainMenu::MainMenu(MainWindow* mainWindow)
{
    // Note:
    // It is OK for the QActions to have their menu as a parent.
    // They won't be used anywhere else.
    // Otherwise we would have to delete them manually.

    // File Menu
    fileMenu = new QMenu("File");
    this->addMenu(fileMenu);

    newProjectAction = new QAction("New Project" , fileMenu);
    fileMenu->addAction(newProjectAction);
    connect(newProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleNewProjectAction);

    fileMenu->addSeparator();

    openProjectAction = new QAction("Open Project" , fileMenu);
    fileMenu->addAction(openProjectAction);
    connect(openProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleOpenProjectAction);

    saveProjectAction = new QAction("Save Project" , fileMenu);
    fileMenu->addAction(saveProjectAction);
    connect(saveProjectAction, &QAction::triggered,
            mainWindow, &MainWindow::handleSaveProjectAction);

    saveProjectAsAction = new QAction("Save Project As..." , fileMenu);
    fileMenu->addAction(saveProjectAsAction);
    connect(saveProjectAsAction, &QAction::triggered,
            mainWindow, &MainWindow::handleSaveProjectAsAction);

    fileMenu->addSeparator();

    exitAction = new QAction("Exit" , fileMenu);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered,
            mainWindow, &MainWindow::handleExitAction,
            Qt::QueuedConnection);

    // Edit Menu
    editMenu = new QMenu("Edit");
    this->addMenu(editMenu);

    auto createNodeMenu = editMenu->addMenu("Create Node");
    QMapIterator<NodeType, QString> i(nodeStrings);

    // Populate menu with submenus aka categories
    QMap<NodeCategory, QMenu*> categories;

    {
        QMapIterator<NodeCategory, QString> i(categoryStrings);
        while (i.hasNext())
        {
            i.next();
            auto submenu = createNodeMenu->addMenu(categoryStrings[i.key()]);
            categories[i.key()] = submenu;
            submenu->setFixedWidth(120);
        }
    }
    auto graph = mainWindow->getNodeGraph();
    while (i.hasNext())
    {
        i.next();
        auto a = new QAction();
        createNodeActions.push_back(a);
        a->setText(i.value());
        auto t = i.key();
        categories[getPropertiesForType(t).category]->addAction(a);

        QObject::connect(
                    a,
                    &QAction::triggered,
                    graph,
                    [graph, t]{ graph->createNode(
                        t,
                        QPoint(graph->lastCreatedNodePos.x(),
                               graph->lastCreatedNodePos.y()) +
                        QPoint(100, 30)); });
    }

    editMenu->addSeparator();

    preferencesAction = new QAction("Preferences..." , editMenu);
    editMenu->addAction(preferencesAction);
    connect(preferencesAction, &QAction::triggered,
            mainWindow, &MainWindow::handlePreferencesAction);

    // View Menu
    viewMenu = new QMenu("View");
    this->addMenu(viewMenu);

    viewMenu->addAction(mainWindow->nodeGraphDockWidget->toggleViewAction());
    viewMenu->addAction(mainWindow->propertiesViewDockWidget->toggleViewAction());

    viewMenu->addSeparator();

    // Help Menu
    helpMenu = new QMenu("Help");
    this->addMenu(helpMenu);

    shortcutsAction = new QAction("Shortcuts", helpMenu);
    helpMenu->addAction(shortcutsAction);
    connect(shortcutsAction, &QAction::triggered,
            mainWindow, &MainWindow::handleShortcutsAction);
}

MainMenu::~MainMenu()
{
    foreach (auto& action, createNodeActions)
    {
        delete action;
    }
}
