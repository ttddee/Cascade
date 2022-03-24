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
#include <QJsonObject>
#include <QJsonArray>

#include "log.h"

ISFManager& ISFManager::getInstance()
{
    static ISFManager instance;

    return instance;
}

const std::vector<unsigned int>& ISFManager::getShaderCode(const QString& nodeName)
{
    return isfShaderCode.at(nodeName);
}

const std::set<QString>& ISFManager::getCategories() const
{
    return isfNodeCategories;
}

const std::map<QString, NodeInitProperties>& ISFManager::getNodeProperties() const
{
    return isfNodeProperties;
}

const QString ISFManager::getCategoryPerNode(const QString &name) const
{
    return isfCategoryPerNode.at(name);
}

void ISFManager::setUp()
{
    QDir dir("isf");
    dir.setNameFilters(QStringList("*.fs"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    int success = 0;
    int failure = 0;

    QStringList fileList = dir.entryList();
    for (int i = 0; i < fileList.count(); ++i)
    {
        QString fileName = fileList[i];

        QFile file(dir.path() + "/" + fileName);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            CS_LOG_WARNING("Could not read ISF shader file.");
        }
        else
        {
            QString name = "ISF " + fileName.split(".").first();

            QString contents;
            contents = file.readAll();
            QStringList split = contents.split("*/");
            QString json = split.first();
            json.remove(0, 2);

            QByteArray shaderData = json.toUtf8();
            QJsonDocument jsonData(QJsonDocument::fromJson(shaderData));

            // Convert and compile shader
            QString shader = convertISFShaderToCompute(split.last(), jsonData);

            if (name == "ISF ASCII Art")
                CS_LOG_CONSOLE(shader);

            if (compiler.compileGLSLFromCode(shader.toLocal8Bit().data(), "comp"))
            {
                success++;

                // Populate categories
                QJsonObject propObject = jsonData.object();
                QJsonArray categoriesArray = propObject.value("CATEGORIES").toArray();
                QString categoryName = categoriesArray.first().toString();
                isfNodeCategories.insert(categoryName);
                isfCategoryPerNode[name] = categoryName;

                // Create properties for the creation of the node
                isfNodeProperties[name] = createISFNodeProperties(jsonData, name);

                this->isfShaderCode[name] = compiler.getSpirV();
            }
            else
            {
                CS_LOG_WARNING(QString::fromStdString(compiler.getError()));
                failure++;
            }
        }
    }
    CS_LOG_INFO("Successfully compiled ISF shaders:");
    CS_LOG_INFO(QString::number(success));
    CS_LOG_WARNING("Compilation failed for:");
    CS_LOG_WARNING(QString::number(failure));
}

NodeInitProperties ISFManager::createISFNodeProperties(
        const QJsonDocument& json,
        const QString& name)
{
    QJsonObject propObject = json.object();

    NodeInitProperties props =
    {
        NODE_TYPE_ISF,
        name,
        NODE_CATEGORY_ISF,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        createUIElementsFromJson(name, propObject),
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noop_comp.spv",
        1
    };

    return props;
}

const std::vector<std::pair<UIElementType, QString>> ISFManager::createUIElementsFromJson(
        const QString& nodeName,
        const QJsonObject &json)
{
    std::vector<std::pair<UIElementType, QString>> elements;
    elements.push_back({ UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeName });

    QJsonArray inputsArray = json.value("INPUTS").toArray();

    for (int i = 0; i < inputsArray.size(); i++)
    {
        QJsonObject property = inputsArray.at(i).toObject();
        QString name = property["NAME"].toString();
        QString propType = property["TYPE"].toString();

        if (propType == "float")
        {
            elements.push_back(
                {
                    UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                    name + "," +
                    QString::number(property["MIN"].toDouble()) +
                    "," +
                    QString::number(property["MAX"].toDouble()) +
                    ",0.01," +
                    QString::number(property["DEFAULT"].toDouble())
                });
            CS_LOG_CONSOLE(property["NAME"].toString());
            CS_LOG_CONSOLE(QString::number(property["DEFAULT"].toDouble()));
        }
    }
    //auto credit = json.value("CREDIT").toObject();
    CS_LOG_CONSOLE(json.value("CREDIT").toString());
    elements.push_back({ UI_ELEMENT_TYPE_TEXTBOX, "Credit: " + json.value("CREDIT").toString() });
    return elements;
}

const QString ISFManager::convertISFShaderToCompute(
        QString& shader,
        const QJsonDocument& properties)
{
    QString compute(
        "#version 430\n"
        "\n"
        "layout (local_size_x = 16, local_size_y = 16) in;\n"
        "layout (binding = 0, rgba32f) uniform readonly image2D inputBack;\n"
        "layout (binding = 1, rgba32f) uniform readonly image2D inputFront;\n"
        "layout (binding = 2, rgba32f) uniform image2D resultImage;\n"
        "\n"
        "ivec2 imgSize = imageSize(inputBack);\n"
        "ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);\n"
        "vec2 pixelCoordsNorm = pixelCoords / imgSize;\n"
        "\n"
        "vec4 result;\n"
        "\n"
        "vec4 imageLoadNorm(vec2 uv)\n"
        "{\n"
        "    return imageLoad(inputBack, ivec2(imgSize * uv));\n"
        "}\n"
        "\n"
        "layout(set = 0, binding = 3) uniform InputBuffer\n"
        "{\n");
    QJsonObject propObject = properties.object();
    QJsonArray inputsArray = propObject.value("INPUTS").toArray();

    int offset = 0;
    std::vector<QString> floatProps;
    std::vector<QString> colorProps;
    std::vector<QString> boolProps;
    for (int i = 0; i < inputsArray.size(); i++)
    {
        QJsonObject property = inputsArray.at(i).toObject();
        QString name = property["NAME"].toString();
        if (property["TYPE"] == "float")
        {
            floatProps.push_back(name);
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + ";\n");
            offset += 4;
        }
        if (property["TYPE"] == "color")
        {
            colorProps.push_back(name);
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "R;\n");
            offset += 4;
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "G;\n");
            offset += 4;
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "B;\n");
            offset += 4;
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "A;\n");
            offset += 4;
        }
        if (property["TYPE"] == "bool")
        {
            boolProps.push_back(name);
            compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + ";\n");
            offset += 4;
        }
    }
    compute.append("} sb;\n\n");
    for (auto& f : floatProps)
    {
        compute.append("float " + f + " = sb." + f + ";\n");
    }
    for (auto& c : colorProps)
    {
        compute.append("vec4 " + c + " = vec4(sb." + c + "R, sb." + c + "G, sb." + c + "B, sb." + c + "A);\n");
    }
    for (auto& b : boolProps)
    {
        compute.append("bool " + b + " = bool(sb." + b + ");\n");
    }

    shader.replace("IMG_THIS_PIXEL(inputImage)", "imageLoad(inputBack, pixelCoords).rgba", Qt::CaseSensitive);
    shader.replace("IMG_NORM_PIXEL(inputImage, ", "imageLoadNorm(", Qt::CaseSensitive);
    shader.replace("RENDERSIZE", "imgSize", Qt::CaseSensitive);
    shader.replace("gl_FragColor", "result", Qt::CaseSensitive);
    shader.replace("gl_FragCoord", "pixelCoordsNorm", Qt::CaseSensitive);
    shader.chop(2);

    compute.append(shader);
    compute.append(
        "imageStore(resultImage, pixelCoords, result);\n"
        "}");

    return compute;
}
