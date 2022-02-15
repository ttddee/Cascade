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

#include "projectmanager.h"

#include <QString>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonArray>

#include "log.h"
#include "nodedefinitions.h"

ProjectManager& ProjectManager::getInstance()
{
    static ProjectManager instance;

    return instance;
}

void ProjectManager::setUp(NodeGraph* ng)
{
    nodeGraph = ng;

    connect(nodeGraph, &NodeGraph::projectIsDirty,
            this, &ProjectManager::handleProjectIsDirty);
}

void ProjectManager::createNewProject()
{
    nodeGraph->createProject();
}

void ProjectManager::loadProject()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("CSC Project (*.csc)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList files;
    if (dialog.exec())
    {
        files = dialog.selectedFiles();
    }

    QFile loadFile(files.first());

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        CS_LOG_WARNING("Couldn't open project file.");
    }

    QByteArray projectData = loadFile.readAll();

    QJsonDocument projectDocument(QJsonDocument::fromJson(projectData));

    QJsonObject jsonProject = projectDocument.object();
    QJsonArray jsonNodeGraph = jsonProject.value("nodegraph").toArray();
    QJsonObject jsonNodesHeading = jsonNodeGraph.at(0).toObject();
    QJsonArray jsonNodesArray = jsonNodesHeading.value("nodes").toArray();
    QJsonObject jsonConnectionsHeading = jsonNodeGraph.at(1).toObject();
    QJsonArray jsonConnectionsArray = jsonConnectionsHeading.value("connections").toArray();

    nodeGraph->loadProject(jsonNodesArray, jsonConnectionsArray);

    currentProjectPath = files.first();
    currentProject = files.first().split("/").last();

    projectIsDirty = false;
    updateProjectName();
}

void ProjectManager::saveProject()
{
    if(projectIsDirty && currentProjectPath != "" && currentProject != "")
    {
        project.setObject(getJsonFromNodeGraph());
        writeJsonToDisk(project, currentProjectPath);

        projectIsDirty = false;
        updateProjectName();
    }
    else if(currentProject == "")
    {
        saveProjectAs();
    }
}

void ProjectManager::saveProjectAs()
{
    QString path;

    QFileDialog dialog;
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setNameFilter(tr("CSC Project (*.csc)"));
    dialog.setDefaultSuffix("csc");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QString f;
    if (dialog.exec())
    {
        auto list = dialog.selectedFiles();
        f = list[0];
    }
    if (f.isEmpty())
        return;
    else
        path = f;

    project.setObject(getJsonFromNodeGraph());
    writeJsonToDisk(project, path);

    currentProjectPath = path;
    currentProject = path.split("/").last();

    projectIsDirty = false;
    updateProjectName();
}

void ProjectManager::handleProjectIsDirty()
{
    projectIsDirty = true;
    updateProjectName();
}

void ProjectManager::updateProjectName()
{
    QString dirt;

    if (projectIsDirty)
        dirt = "*";
    else
        dirt = "";

    emit projectTitleChanged(currentProject + dirt);
}

void ProjectManager::writeJsonToDisk(const QJsonDocument& project,
                                    const QString &path)
{
    // check path
    QFile jsonFile = QFile(path);
    if (!jsonFile.exists())
    {
        CS_LOG_WARNING("Could not write json file to disk.");
    }

    // save to disk
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(project.toJson());
}

QJsonObject ProjectManager::getJsonFromNodeGraph()
{
    QJsonArray jsonNodeGraph;
    nodeGraph->getNodeGraphAsJson(jsonNodeGraph);

    QJsonObject jsonProject {
        { "nodegraph", jsonNodeGraph },
        { "cascade-version", QString("%1.%2.%3")
                    .arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD) }

    };

    return jsonProject;
}
