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

#ifndef ISFMANAGER_H
#define ISFMANAGER_H

#include <QObject>
#include <QJsonDocument>

#include "shadercompiler/SpvShaderCompiler.h"

class ISFManager : public QObject
{
    Q_OBJECT

public:
    static ISFManager& getInstance();
    ISFManager(ISFManager const&) = delete;
    void operator=(ISFManager const&) = delete;

    void setUp();

private:
    ISFManager() {}

    bool convertISFToCompute(const QString& shader);

    SpvCompiler compiler;

    std::map<QString, QJsonDocument> isfProperties;
    std::map<QString, std::vector<unsigned int>> isfShaderCode;
};

#endif // ISFMANAGER_H
