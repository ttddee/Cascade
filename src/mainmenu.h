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

#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenuBar>

class MainWindow;

class MainMenu : public QMenuBar
{
    Q_OBJECT
public:
    MainMenu(MainWindow* mainWindow);

    ~MainMenu();

private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction* newProjectAction;
    QAction* openProjectAction;
    QAction* saveProjectAction;
    QAction* saveProjectAsAction;
    QAction* exitAction;
    QAction* preferencesAction;
    QAction* toggleNodeGraphAction;
    QAction* togglePropertiesAction;
    QAction* aboutAction;

    std::vector<QAction*> createNodeActions;
};

#endif // MAINMENU_H
