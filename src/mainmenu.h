#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMenuBar>

class MainWindow;

class MainMenu : public QMenuBar
{
    Q_OBJECT
public:
    MainMenu(MainWindow* mainWindow);

private:
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction* newProjectAction;
    QAction* openProjectAction;
    QAction* saveProjectAction;
    QAction* saveProjectAsAction;
    QAction* exitAction;
    QAction* toggleNodeGraphAction;
    QAction* togglePropertiesAction;
    QAction* shortcutsAction;

signals:
    void newProjectActionTriggered();
    void openProjectActionTriggered();
    void saveProjectActionTriggered();
    void saveProjectAsActionTriggered();
    void exitActionTriggered();
    void shortcutsActionTriggered();
};

#endif // MAINMENU_H
