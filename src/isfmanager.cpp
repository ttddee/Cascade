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

namespace Cascade {

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

            //if (name == "ISF VHS Glitch")
                //CS_LOG_INFO(shader);

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
                isfNodeProperties[name] = createISFNodeProperties(propObject, name);

                this->isfShaderCode[name] = compiler.getSpirV();
            }
            else
            {
                CS_LOG_INFO("Compilation failed for:" + name);
                CS_LOG_WARNING(QString::fromStdString(compiler.getError()));
                failure++;
            }
        }
    }
    CS_LOG_INFO("Successfully compiled ISF shaders:" + QString::number(success));
    CS_LOG_WARNING("Compilation failed for:" + QString::number(failure));
}

NodeInitProperties ISFManager::createISFNodeProperties(
        const QJsonObject& json,
        const QString& name)
{
    NodeInitProperties props =
    {
        NODE_TYPE_ISF,
        name,
        NODE_CATEGORY_ISF,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        createUIElementsFromJson(name, json),
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noop_comp.spv",
        1
    };
    return props;
}

const int ISFManager::getRenderpassesFromJson(
        const QJsonObject &json) const
{
    int passes = 1;
//    QJsonArray passesArray = json.value("PASSES").toArray();
//    for (auto pass : passesArray)
//    {
//        passes++;
//    }
    return passes;
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
        QString label = property["LABEL"].toString();
        // If control is missing the label, use the name
        if (label == "")
        {
            label = property["NAME"].toString();
        }
        QString propType = property["TYPE"].toString();

        if (propType == "float")
        {
            elements.push_back(
                {
                    UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                    label + "," +
                    QString::number(property["MIN"].toDouble()) +
                    "," +
                    QString::number(property["MAX"].toDouble()) +
                    ",0.01," +
                    QString::number(property["DEFAULT"].toDouble())
                });
        }
        if (propType == "color")
        {
            auto array = property["DEFAULT"].toArray();
            elements.push_back(
                {
                    UI_ELEMENT_TYPE_COLOR_BUTTON,
                    label +
                    "," +
                    QString::number(array.at(0).toDouble()) +
                    "," +
                    QString::number(array.at(1).toDouble()) +
                    "," +
                    QString::number(array.at(2).toDouble()) +
                    "," +
                    QString::number(array.at(3).toDouble())
                });
        }
        if (propType == "bool")
        {
            elements.push_back(
                {
                   UI_ELEMENT_TYPE_CHECKBOX,
                   label + "," + QString::number(property["DEFAULT"].toInt())
                });
        }
        if (propType == "int")
        {
            elements.push_back(
                {
                   UI_ELEMENT_TYPE_SLIDER_BOX_INT,
                   label + "," +
                   QString::number(property["MIN"].toInt()) +
                   "," +
                   QString::number(property["MAX"].toInt()) +
                   ",1," +
                   QString::number(property["DEFAULT"].toInt())
                });
        }
        if (propType == "point2D")
        {
            auto min = property["MIN"].toArray();
            auto max = property["MAX"].toArray();
            auto def = property["DEFAULT"].toArray();
            elements.push_back(
                {
                    UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                    label + " X," +
                    QString::number(min.first().toDouble()) +
                    "," +
                    QString::number(max.first().toDouble()) +
                    ",0.01," +
                    QString::number(def.first().toDouble())
                });
            elements.push_back(
                {
                    UI_ELEMENT_TYPE_SLIDER_BOX_DOUBLE,
                    label + " Y," +
                    QString::number(min.last().toDouble()) +
                    "," +
                    QString::number(max.last().toDouble()) +
                    ",0.01," +
                    QString::number(def.last().toDouble())
                });
        }
        if (propType == "long")
        {
            auto array = property["LABELS"].toArray();
            QString labels;
            for (int i = 0; i < array.size(); ++i)
            {
                labels.append(array[i].toString() + ",");
            }
            elements.push_back(
                {
                   UI_ELEMENT_TYPE_COMBOBOX,
                   label + "," +
                   labels +
                   QString::number(property["DEFAULT"].toInt())
                });
        }
    }
    elements.push_back({ UI_ELEMENT_TYPE_TEXTBOX, "CREDIT: " + json.value("CREDIT").toString() });
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
        "vec2 pixelCoordsNorm = vec2(float(pixelCoords.x) / imgSize.x, float(pixelCoords.y) / imgSize.y);\n"
        "vec2 imgSizeNorm = 1.0 / imgSize;\n"
        "\n"
        "vec4 result;\n"
        "\n"
        "vec4 csImageLoad(vec2 coords)\n"
        "{\n"
        "    return imageLoad(inputBack, ivec2(coords));\n"
        "}\n"
        "\n"
        "vec4 csImageLoadNorm(vec2 uv)\n"
        "{\n"
        "    return imageLoad(inputBack, ivec2(imgSize * uv));\n"
        "}\n"
        "\n"
        );
    QJsonObject propObject = properties.object();
    QJsonArray inputsArray = propObject.value("INPUTS").toArray();
    // Remove inputImage
    int index = getIndexFromArray(inputsArray, "inputImage");
    if (index >= 0)
    {
        inputsArray.removeAt(index);
    }
    if (inputsArray.size() > 0)
    {
        // Remove whitespace at start and end
        shader = shader.trimmed();

        compute.append("layout(set = 0, binding = 3) uniform InputBuffer\n"
                       "{\n");

        int offset = 0;
        std::vector<QString> floatProps;
        std::vector<QString> colorProps;
        std::vector<QString> boolProps;
        std::vector<QString> pointProps;
        std::vector<QString> intProps;
        std::vector<QString> longProps;
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
            if (property["TYPE"] == "point2D")
            {
                pointProps.push_back(name);
                compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "X;\n");
                offset += 4;
                compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + "Y;\n");
                offset += 4;
            }
            if (property["TYPE"] == "int")
            {
                intProps.push_back(name);
                compute.append("    layout(offset = " + QString::number(offset) + ") float " + name + ";\n");
                offset += 4;
            }
            if (property["TYPE"] == "long")
            {
                longProps.push_back(name);
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
        for (auto& p : pointProps)
        {
            compute.append("vec2 " + p + " = vec2(sb." + p + "X, sb." + p + "Y);\n");
        }
        for (auto& i : intProps)
        {
            compute.append("int " + i + " = int(sb." + i + ");\n");
        }
        for (auto& l : longProps)
        {
            compute.append("int " + l + " = int(sb." + l + ");\n");
        }
    }

    // Replace
    shader.replace("IMG_THIS_PIXEL(inputImage)", "imageLoad(inputBack, pixelCoords).rgba", Qt::CaseSensitive);
    shader.replace("IMG_PIXEL(inputImage,", "csImageLoad(", Qt::CaseSensitive);
    shader.replace("IMG_NORM_PIXEL(inputImage,", "csImageLoadNorm(", Qt::CaseSensitive);
    shader.replace("RENDERSIZE", "imgSize", Qt::CaseSensitive);
    shader.replace("gl_FragColor", "result", Qt::CaseSensitive);
    shader.replace("gl_FragCoord", "pixelCoords", Qt::CaseSensitive);
    shader.replace("isf_FragNormCoord", "pixelCoordsNorm", Qt::CaseSensitive);
    shader.replace("in vec2 left_coord;", "vec2 left_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(-imgSizeNorm.x, 0)), 0.0, 1.0);\n");
    shader.replace("in vec2 right_coord;", "vec2 right_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(imgSizeNorm.x, 0)), 0.0, 1.0);\n");
    shader.replace("in vec2 above_coord;", "vec2 above_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(0, imgSizeNorm.y)), 0.0, 1.0);\n");
    shader.replace("in vec2 below_coord;", "vec2 below_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(0, -imgSizeNorm.y)), 0.0, 1.0);\n");
    shader.replace("in vec2 lefta_coord;", "vec2 lefta_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(-imgSizeNorm.x, imgSizeNorm.x)), 0.0, 1.0);\n");
    shader.replace("in vec2 righta_coord;", "vec2 righta_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(imgSizeNorm.x, imgSizeNorm.x)), 0.0, 1.0);\n");
    shader.replace("in vec2 leftb_coord;", "vec2 leftb_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(-imgSizeNorm.x, -imgSizeNorm.x)), 0.0, 1.0);\n");
    shader.replace("in vec2 rightb_coord;", "vec2 rightb_coord = clamp(vec2(pixelCoordsNorm.xy + vec2(imgSizeNorm.x, -imgSizeNorm.x)), 0.0, 1.0);\n");
    // TODO: Make this a control
    shader.replace("TIME", "1000", Qt::CaseSensitive);

    // Chop the closing brace
    shader.chop(1);

    compute.append(shader);
    compute.append(
        "\n"
        "imageStore(resultImage, pixelCoords, result);\n"
        "}");

    return compute;
}

const int ISFManager::getIndexFromArray(const QJsonArray& array, const QString& value) const
{
    for (int i = 0; i < array.size(); ++i)
    {
        if (array[i].toObject()["NAME"].toString() == value)
        {
            return i;
        }
    }
    return -1;
}

} // namespace Cascade
