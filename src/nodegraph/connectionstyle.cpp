#include "connectionstyle.h"

#include "stylecollection.h"

#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValueRef>
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

#include <QtCore/QJsonArray>

#include <QDebug>

#include <random>

using Cascade::NodeGraph::ConnectionStyle;

inline void initResources() { Q_INIT_RESOURCE(resources); }

ConnectionStyle::ConnectionStyle()
{
    // Explicit resources inialization for preventing the static initialization
    // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
    initResources();

    // This configuration is stored inside the compiled unit and is loaded statically
    loadJsonFile(":/style/nodegraphstyle.json");
}


ConnectionStyle::ConnectionStyle(QString jsonText)
{
    loadJsonFile(":/style/nodegraphstyle.json");
    loadJsonText(jsonText);
}


void ConnectionStyle::setConnectionStyle(QString jsonText)
{
    ConnectionStyle style(jsonText);

    StyleCollection::setConnectionStyle(style);
}

#ifdef STYLE_DEBUG
#define CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(v, variable) { \
if (v.type() == QJsonValue::Undefined || \
                                             v.type() == QJsonValue::Null) \
        qWarning() << "Undefined value for parameter:" << #variable; \
}
#else
#define CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(v, variable)
#endif


#define CONNECTION_VALUE_EXISTS(v) \
(v.type() != QJsonValue::Undefined && \
                                          v.type() != QJsonValue::Null)

#define CONNECTION_STYLE_READ_COLOR(values, variable)  { \
    auto valueRef = values[#variable]; \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
    if (CONNECTION_VALUE_EXISTS(valueRef)) {\
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
} \
}

#define CONNECTION_STYLE_READ_FLOAT(values, variable)  { \
auto valueRef = values[#variable]; \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
    if (CONNECTION_VALUE_EXISTS(valueRef)) \
    variable = valueRef.toDouble(); \
}

#define CONNECTION_STYLE_READ_BOOL(values, variable)  { \
auto valueRef = values[#variable]; \
    CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
    if (CONNECTION_VALUE_EXISTS(valueRef)) \
    variable = valueRef.toBool(); \
}

void ConnectionStyle::loadJsonFile(QString styleFile)
{
    QFile file(styleFile);

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open file " << styleFile;

        return;
    }

    loadJsonFromByteArray(file.readAll());
}


void ConnectionStyle::loadJsonText(QString jsonText)
{
    loadJsonFromByteArray(jsonText.toUtf8());
}


void ConnectionStyle::loadJsonFromByteArray(QByteArray const &byteArray)
{
    QJsonDocument json(QJsonDocument::fromJson(byteArray));

    QJsonObject topLevelObject = json.object();

    QJsonValueRef nodeStyleValues = topLevelObject["ConnectionStyle"];

    QJsonObject obj = nodeStyleValues.toObject();

    CONNECTION_STYLE_READ_COLOR(obj, ConstructionColor);
    CONNECTION_STYLE_READ_COLOR(obj, NormalColor);
    CONNECTION_STYLE_READ_COLOR(obj, BackColor);
    CONNECTION_STYLE_READ_COLOR(obj, FrontColor);
    CONNECTION_STYLE_READ_COLOR(obj, ResultColor);
    CONNECTION_STYLE_READ_COLOR(obj, SelectedColor);
    CONNECTION_STYLE_READ_COLOR(obj, SelectedHaloColor);
    CONNECTION_STYLE_READ_COLOR(obj, HoveredColor);

    CONNECTION_STYLE_READ_FLOAT(obj, LineWidth);
    CONNECTION_STYLE_READ_FLOAT(obj, ConstructionLineWidth);
    CONNECTION_STYLE_READ_FLOAT(obj, PointDiameter);
}


QColor ConnectionStyle::constructionColor() const
{
    return ConstructionColor;
}


QColor ConnectionStyle::normalColor() const
{
    return NormalColor;
}


QColor ConnectionStyle::normalColor(const QString& portName) const
{
    if (portName == "RGBA Back")
        return BackColor;
    else if (portName == "RGBA Front")
        return FrontColor;
    else if (portName == "Result")
        return ResultColor;
    else
        return NormalColor;
}


QColor ConnectionStyle::selectedColor() const
{
    return SelectedColor;
}


QColor ConnectionStyle::selectedHaloColor() const
{
    return SelectedHaloColor;
}


QColor ConnectionStyle::hoveredColor() const
{
    return HoveredColor;
}


float ConnectionStyle::lineWidth() const
{
    return LineWidth;
}


float ConnectionStyle::constructionLineWidth() const
{
    return ConstructionLineWidth;
}


float ConnectionStyle::pointDiameter() const
{
    return PointDiameter;
}

