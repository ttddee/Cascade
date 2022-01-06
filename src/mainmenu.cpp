#include "mainmenu.h"

#include "mainwindow.h"

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
