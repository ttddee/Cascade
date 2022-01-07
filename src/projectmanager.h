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

#include "nodegraph.h"

class ProjectManager : public QObject
{
    Q_OBJECT

public:
    static ProjectManager& getInstance();
    ProjectManager(ProjectManager const&) = delete;
    void operator=(ProjectManager const&) = delete;

    void setUp(NodeGraph* ng);

    void saveProject();
    void saveProjectAs();

private:
    ProjectManager() {}
    void updateProjectName();
    void writeJsonToDisk(const QJsonDocument& project,
                        const QString& path);
    QJsonObject getJsonFromNodeGraph();

    NodeGraph* nodeGraph;

    QJsonDocument project;
    QString currentProjectPath;
    QString currentProject;
    bool projectIsDirty = true;

signals:
    void projectTitleChanged(const QString& t);

public slots:
    void handleProjectIsDirty();
};

#endif // PROJECTMANAGER_H
