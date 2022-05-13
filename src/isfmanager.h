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

#include <set>

#include <QObject>
#include <QJsonDocument>

#include "shadercompiler/SpvShaderCompiler.h"
#include "nodegraph/nodedefinitions.h"

namespace Cascade {

class ISFManager : public QObject
{
    Q_OBJECT

public:
    static ISFManager& getInstance();
    ISFManager(ISFManager const&) = delete;
    void operator=(ISFManager const&) = delete;

    void setUp();

    const std::vector<unsigned int>& getShaderCode(const QString& nodeName);
    const std::set<QString>& getCategories() const;
    const std::map<QString, NodeInitProperties>& getNodeProperties() const;
    const QString getCategoryPerNode(const QString& name) const;

private:
    ISFManager() {}

    const QString convertISFShaderToCompute(
            QString& shader,
            const QJsonDocument& properties);

    NodeInitProperties createISFNodeProperties(
            const QJsonObject& json,
            const QString& name);

    const std::vector<std::pair<UIElementType, QString>> createUIElementsFromJson(
            const QString& nodeName,
            const QJsonObject& json);

    const int getRenderpassesFromJson(const QJsonObject& json) const;

    const int getIndexFromArray(const QJsonArray& array, const QString& value) const;

    SpvCompiler mCompiler;

    std::map<QString, QJsonDocument> mIsfProperties;
    std::map<QString, std::vector<unsigned int>> mIsfShaderCode;
    std::set<QString> mIsfNodeCategories;
    std::map<QString, NodeInitProperties> mIsfNodeProperties;
    std::map<QString, QString> mIsfCategoryPerNode;
};

} // namespace Cascade

#endif // ISFMANAGER_H
