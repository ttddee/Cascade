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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidgetAction>
#include <QSettings>

#include "docking/DockManager.h"
#include "docking/DockAreaWidget.h"
#include "docking/DockWidget.h"

#include "windowmanager.h"
#include "rendermanager.h"
#include "vulkanview.h"
#include "nodegraph.h"
#include "propertiesview.h"
#include "viewerstatusbar.h"
#include "mainmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ads::CDockWidget* nodeGraphDockWidget;
    ads::CDockWidget* propertiesViewDockWidget;

    ~MainWindow();

private:
    void saveProjectAs();
    void displayShortcuts();

    Ui::MainWindow *ui;
    VulkanView* vulkanView;
    NodeGraph* nodeGraph;
    PropertiesView* propertiesView;
    ViewerStatusBar* viewerStatusBar;

    WindowManager* windowManager;
    RenderManager* renderManager;

    ads::CDockManager* dockManager;

    MainMenu* mainMenu;

public slots:
    void handleRendererHasBeenCreated();
    void handleNoGPUFound();
    void handleDeviceLost();

    // Main Menu
    void handleNewProjectAction();
    void handleOpenProjectAction();
    void handleSaveProjectAction();
    void handleSaveProjectAsAction();
    void handleExitAction();
    void handleShortcutsAction();

};
#endif // MAINWINDOW_H
