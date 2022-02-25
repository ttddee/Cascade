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

#include "preferencesmanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "log.h"

PreferencesManager& PreferencesManager::getInstance()
{
    static PreferencesManager instance;

    return instance;
}

void PreferencesManager::setUp()
{
    loadPreferences();
}

void PreferencesManager::loadPreferences()
{
    QFile file(":/cascade.prefs");

    if (!file.open(QIODevice::ReadOnly))
    {
        CS_LOG_WARNING("Couldn't open preferences file.");
    }

    QByteArray prefsData = file.readAll();

    QJsonDocument prefsDocument(QJsonDocument::fromJson(prefsData));

    QJsonObject jsonProject = prefsDocument.object();
    QJsonArray jsonPrefs = jsonProject.value("prefs").toArray();
    QJsonObject jsonGeneralHeading = jsonPrefs.at(0).toObject();
    jsonGeneralPrefsArray = jsonGeneralHeading.value("general").toArray();
    QJsonObject jsonKeysHeading = jsonPrefs.at(1).toObject();
    jsonKeysPrefsArray = jsonKeysHeading.value("keys").toArray();
}

const QJsonArray& PreferencesManager::getKeys()
{
    return jsonKeysPrefsArray;
}
