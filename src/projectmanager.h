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

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QWidget>
#include <QJsonDocument>

//#include "nodegraph/nodegraph.h"

namespace Cascade {

class ProjectManager : public QObject
{
    Q_OBJECT

public:
    static ProjectManager& getInstance();
    ProjectManager(ProjectManager const&) = delete;
    void operator=(ProjectManager const&) = delete;

    //void setUp(NodeGraph* ng);

    void createStartupProject();
    void createNewProject();
    void loadProject();
    void saveProject();
    void saveProjectAs();

private:
    ProjectManager() {}
    void updateProjectName();
    void writeJsonToDisk(const QJsonDocument& project,
                        const QString& path);
    QJsonObject getJsonFromNodeGraph();
    bool checkIfDiscardChanges();

    //NodeGraph* mNodeGraph;

    QJsonDocument mProject;
    QString mCurrentProjectPath;
    QString mCurrentProject;
    bool mProjectIsDirty = true;

signals:
    void projectTitleChanged(const QString& t);
    void requestCreateStartupProject();
    void requestCreateNewProject();
    void requestLoadProject(const QJsonArray& jsonNodeGraph);

public slots:
    void handleProjectIsDirty();
};

} // namespace Cascade

#endif // PROJECTMANAGER_H
