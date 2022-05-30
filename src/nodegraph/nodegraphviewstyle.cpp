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
 *
 *  NodeEditor code adapted from:
 *  Dmitry Pinaev et al, Qt Node Editor, (2017), GitHub repository, https://github.com/paceholder/nodeeditor
*/

#include "nodegraphviewstyle.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
#include <QtCore/QJsonArray>

#include <QDebug>

#include "stylecollection.h"

using Cascade::NodeGraph::NodeGraphViewStyle;

inline void initResources() { Q_INIT_RESOURCE(resources); }

NodeGraphViewStyle::NodeGraphViewStyle()
{
    // Explicit resources inialization for preventing the static initialization
    // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
    initResources();

    // This configuration is stored inside the compiled unit and is loaded statically
    loadJsonFile(":/style/nodegraphstyle.json");
}


NodeGraphViewStyle::NodeGraphViewStyle(QString jsonText)
{
    loadJsonText(jsonText);
}


void NodeGraphViewStyle::setStyle(QString jsonText)
{
    NodeGraphViewStyle style(jsonText);

    StyleCollection::setnodeGraphViewStyle(style);
}


#ifdef STYLE_DEBUG
#define FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(v, variable) { \
if (v.type() == QJsonValue::Undefined || \
                                             v.type() == QJsonValue::Null) \
        qWarning() << "Undefined value for parameter:" << #variable; \
}
#else
#define FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(v, variable)
#endif

#define FLOW_VIEW_STYLE_READ_COLOR(values, variable)  { \
auto valueRef = values[#variable]; \
    FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
    if (valueRef.isArray()) { \
        auto colorArray = valueRef.toArray(); \
        std::vector<int> rgb; rgb.reserve(3); \
        for (auto it = colorArray.begin(); it != colorArray.end(); ++it) { \
            rgb.push_back((*it).toInt()); \
    } \
        variable = QColor(rgb[0], rgb[1], rgb[2]); \
} else { \
        variable = QColor(valueRef.toString()); \
} \
}

void NodeGraphViewStyle::loadJsonFile(QString styleFile)
{
    QFile file(styleFile);

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open file " << styleFile;

        return;
    }

    loadJsonFromByteArray(file.readAll());
}


void NodeGraphViewStyle::loadJsonText(QString jsonText)
{
    loadJsonFromByteArray(jsonText.toUtf8());
}


void NodeGraphViewStyle::loadJsonFromByteArray(QByteArray const &byteArray)
{
    QJsonDocument json(QJsonDocument::fromJson(byteArray));

    QJsonObject topLevelObject = json.object();

    QJsonValueRef nodeStyleValues = topLevelObject["NodeGraphViewStyle"];

    QJsonObject obj = nodeStyleValues.toObject();

    FLOW_VIEW_STYLE_READ_COLOR(obj, BackgroundColor);
    FLOW_VIEW_STYLE_READ_COLOR(obj, FineGridColor);
    FLOW_VIEW_STYLE_READ_COLOR(obj, CoarseGridColor);
}
