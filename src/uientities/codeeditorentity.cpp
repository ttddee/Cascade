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

#include "codeeditorentity.h"

#include "../codeeditor/QGLSLCompleter.hpp"
#include "../codeeditor/QGLSLHighlighter.hpp"
#include "../codeeditor/QSyntaxStyle.hpp"

#include "../nodebase.h"

namespace Cascade {

CodeEditorEntity::CodeEditorEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent)
{
    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(QMargins(0,0,0,0));
    layout->setSpacing(15);
    this->setLayout(layout);

    completer = new QGLSLCompleter(this);
    highlighter = new QGLSLHighlighter;
    style = QSyntaxStyle::defaultStyle();

    splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->setObjectName("CodeEditSplitter");

    codeEditor = new QCodeEditor(this);
    codeEditor->setCompleter(completer);
    codeEditor->setHighlighter(highlighter);
    codeEditor->setSyntaxStyle(style);
    splitter->addWidget(codeEditor);

    debugOutput = new QTextEdit(this);
    debugOutput->setReadOnly(true);
    splitter->addWidget(debugOutput);

    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    QString noopShader(
            "#version 430\n"
            "\n"
            "layout (local_size_x = 16, local_size_y = 16) in;\n"
            "layout (binding = 0, rgba32f) uniform readonly image2D inputBack;\n"
            "layout (binding = 1, rgba32f) uniform readonly image2D inputFront;\n"
            "layout (binding = 2, rgba32f) uniform image2D outputImage;\n"
            "\n"
            "ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);\n"
            "\n"
            "ivec2 imageSize = imageSize(inputBack);\n"
            "\n"
            "vec4 pixelBack = imageLoad(inputBack, pixelCoords).rgba;\n"
            "\n"
            "vec4 pixelFront = imageLoad(inputFront, pixelCoords).rgba;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    // Your code goes here\n"
            "\n"
            "    vec4 result = pixelBack;"
            "\n"
            "    imageStore(outputImage, pixelCoords, result);\n"
            "}");

    codeEditor->setText(noopShader);

    connect(codeEditor, &QCodeEditor::textChanged,
            this, &CodeEditorEntity::handleTextChanged);
}

void CodeEditorEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &CodeEditorEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString CodeEditorEntity::getValuesAsString()
{
    return codeEditor->toPlainText();
}

void CodeEditorEntity::loadPropertyValues(const QString &values)
{
    codeEditor->setText(values);
}

void CodeEditorEntity::handleTextChanged()
{
    QByteArray bytes = codeEditor->toPlainText().toLocal8Bit();
    const char* cStr = bytes.data();

    if (compiler.compileGLSLFromCode(cStr, "comp"))
    {
        debugOutput->setText("Done.");
        parentNode->setShaderCode(compiler.getSpirV());

        emit valueChanged();
    }
    else
    {
        debugOutput->setText(QString::fromStdString(compiler.getError()));
    }
    debugOutput->update();
}

void CodeEditorEntity::setParentNode(NodeBase* node)
{
    parentNode = node;
}

} // namespace Cascade

