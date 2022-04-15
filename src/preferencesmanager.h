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

#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include <QObject>
#include <QJsonArray>

namespace Cascade {

struct KeysCategory
{
    QString name;
    // vector instead of map so that the order is preserved
    std::vector<std::pair<QString, QString>> keys;
};

class PreferencesManager : public QObject
{
    Q_OBJECT

public:
    static PreferencesManager& getInstance();
    PreferencesManager(PreferencesManager const&) = delete;
    void operator=(PreferencesManager const&) = delete;

    void setUp();

    const std::vector<KeysCategory>& getKeys();

private:
    PreferencesManager() {}

    void loadPreferences();
    KeysCategory jsonArrayToKeysCategory(
            const QString& name,
            const QJsonArray& arr);

    std::vector<KeysCategory> keyCategories;
};

} // namespace Cascade

#endif // PREFERENCESMANAGER_H
