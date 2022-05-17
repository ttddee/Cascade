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
#include <QMessageBox>

#include "log.h"
#include "nodegraph/nodedefinitions.h"

namespace Cascade {

ProjectManager& ProjectManager::getInstance()
{
    static ProjectManager instance;

    return instance;
}

void ProjectManager::setUp(NodeGraph* ng)
{
    mNodeGraph = ng;

    // Incoming
    connect(mNodeGraph, &NodeGraph::projectIsDirty,
            this, &ProjectManager::handleProjectIsDirty);

    // Outgoing
    connect(this, &ProjectManager::requestCreateStartupProject,
            mNodeGraph, &NodeGraph::handleCreateStartupProject);
    connect(this, &ProjectManager::requestCreateNewProject,
            mNodeGraph, &NodeGraph::handleCreateNewProject);
    connect(this, &ProjectManager::requestLoadProject,
            mNodeGraph, &NodeGraph::handleLoadProject);
}

void ProjectManager::createStartupProject()
{
    emit requestCreateStartupProject();
}

void ProjectManager::createNewProject()
{
    if (checkIfDiscardChanges())
    {
        emit requestCreateNewProject();
    }
}

const bool ProjectManager::checkIfDiscardChanges()
{
    if (mProjectIsDirty)
    {
        QMessageBox box;
        box.setText("The project has been modified.");
        box.setInformativeText("Do you want to discard your changes?");
        box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Cancel);
        box.setMinimumSize(QSize(500, 200));

        int ret = box.exec();
        if (ret == QMessageBox::Yes)
            return true;
    }
    return false;
}

void ProjectManager::loadProject()
{
    if (checkIfDiscardChanges() || !mProjectIsDirty)
    {
        QFileDialog dialog(nullptr);
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setNameFilter(tr("CSC Project (*.csc)"));
        dialog.setViewMode(QFileDialog::Detail);
        QStringList files;
        if (dialog.exec())
        {
            files = dialog.selectedFiles();

            QFile loadFile(files.first());

            if (!loadFile.open(QIODevice::ReadOnly))
            {
                CS_LOG_WARNING("Couldn't open project file.");
            }

            QByteArray projectData = loadFile.readAll();

            QJsonDocument projectDocument(QJsonDocument::fromJson(projectData));

            QJsonObject jsonProject = projectDocument.object();
            QJsonArray jsonNodeGraph = jsonProject.value("nodegraph").toArray();

            emit requestLoadProject(jsonNodeGraph);

            mCurrentProjectPath = files.first();
            mCurrentProject = files.first().split("/").last();

            mProjectIsDirty = false;
            updateProjectName();
        }
    }
}

void ProjectManager::saveProject()
{
    if(mProjectIsDirty && mCurrentProjectPath != "" && mCurrentProject != "")
    {
        mProject.setObject(getJsonFromNodeGraph());
        writeJsonToDisk(mProject, mCurrentProjectPath);

        mProjectIsDirty = false;
        updateProjectName();
    }
    else if(mCurrentProject == "")
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

        if (f.isEmpty())
            return;
        else
            path = f;

        mProject.setObject(getJsonFromNodeGraph());
        writeJsonToDisk(mProject, path);

        mCurrentProjectPath = path;
        mCurrentProject = path.split("/").last();

        mProjectIsDirty = false;
        updateProjectName();
    }
}

void ProjectManager::handleProjectIsDirty()
{
    mProjectIsDirty = true;
    updateProjectName();
}

void ProjectManager::updateProjectName()
{
    QString dirt;

    if (mProjectIsDirty)
        dirt = "*";
    else
        dirt = "";

    emit projectTitleChanged(mCurrentProject + dirt);
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
    //mNodeGraph->getNodeGraphAsJson(jsonNodeGraph);

    QJsonObject jsonProject {
        { "nodegraph", jsonNodeGraph },
        { "cascade-version", QString("%1.%2.%3")
                    .arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD) }

    };

    return jsonProject;
}

} // namespace Cascade
