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

#include "isfmanager.h"

#include <QDir>

#include "log.h"

ISFManager& ISFManager::getInstance()
{
    static ISFManager instance;

    return instance;
}

void ISFManager::setUp()
{
    QDir dir("isf");
    dir.setNameFilters(QStringList("*.fs"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    for (int i = 0; i < fileList.count(); ++i)
    {
        //CS_LOG_CONSOLE(fileList[i]);
        QString fileName = fileList[i];

        QFile file(dir.path() + "/" + fileName);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            CS_LOG_WARNING("Could not read ISF shader file.");
        }
        else
        {
            QString key = fileName.split(".").first();

            QString contents;
            contents = file.readAll();
            QStringList split = contents.split("*/");
            QString json = split.first();
            json.remove(0, 2);

            QByteArray shaderData = json.toUtf8();
            QJsonDocument jsonData(QJsonDocument::fromJson(shaderData));
            isfProperties[key] = jsonData;

            QString shader = split.last();

            if (compiler.compileGLSLFromCode(shader.toLocal8Bit().data(), "comp"))
            {
                CS_LOG_INFO("ISF shader compiled successfully.")
            }
            else
            {
                CS_LOG_WARNING(QString::fromStdString(compiler.getError()));
            }

            //CS_LOG_CONSOLE(json);
            //CS_LOG_CONSOLE(shader);

        }
    }
    //CS_LOG_INFO("Loaded ISF shaders:");
    //CS_LOG_INFO(QString::number(isfFiles.size()));

    //CS_LOG_INFO(isfFiles.at("3d Rotate"));
}

bool ISFManager::convertISFToCompute(const QString& shader)
{

}
