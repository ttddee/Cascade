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

#include "../nodegraph/nodebase.h"

namespace Cascade {

CodeEditorEntity::CodeEditorEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent)
{
    mLayout = new QVBoxLayout();
    mLayout->setAlignment(Qt::AlignTop);
    mLayout->setContentsMargins(QMargins(0,0,0,0));
    mLayout->setSpacing(15);
    this->setLayout(mLayout);

    mCompleter = new QGLSLCompleter(this);
    mHighlighter = new QGLSLHighlighter;
    mStyle = QSyntaxStyle::defaultStyle();

    mSplitter = new QSplitter(this);
    mSplitter->setOrientation(Qt::Vertical);
    mSplitter->setObjectName("CodeEditSplitter");

    mCodeEditor = new QCodeEditor(this);
    mCodeEditor->setCompleter(mCompleter);
    mCodeEditor->setHighlighter(mHighlighter);
    mCodeEditor->setSyntaxStyle(mStyle);
    mSplitter->addWidget(mCodeEditor);

    mDebugOutput = new QTextEdit(this);
    mDebugOutput->setReadOnly(true);
    mSplitter->addWidget(mDebugOutput);

    mSplitter->setStretchFactor(0, 3);
    mSplitter->setStretchFactor(1, 1);

    mLayout->addWidget(mSplitter);

    QString noopShader(
            "#version 430\n"
            "\n"
            "layout (local_size_x = 16, local_size_y = 16) in;\n"
            "layout (binding = 0, rgba32f) uniform readonly image2D inputBack;\n"
            "layout (binding = 1, rgba32f) uniform readonly image2D inputFront;\n"
            "layout (binding = 2, rgba32f) uniform image2D outputImage;\n"
            "\n"
            "ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);\n"
            "ivec2 imgSize = imageSize(inputBack);\n"
            "vec2 pixelCoordsNorm = vec2(float(pixelCoords.x) / imgSize.x, float(pixelCoords.y) / imgSize.y);\n"
            "\n"
            "vec4 pixelBack = imageLoad(inputBack, pixelCoords).rgba;\n"
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

    mCodeEditor->setText(noopShader);

    connect(mCodeEditor, &QCodeEditor::textChanged,
            this, &CodeEditorEntity::handleTextChanged);
}

void CodeEditorEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &CodeEditorEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString CodeEditorEntity::getValuesAsString()
{
    return mCodeEditor->toPlainText();
}

void CodeEditorEntity::loadPropertyValues(const QString &values)
{
    mCodeEditor->setText(values);
}

void CodeEditorEntity::handleTextChanged()
{
    QByteArray bytes = mCodeEditor->toPlainText().toLocal8Bit();
    const char* cStr = bytes.data();

    if (mCompiler.compileGLSLFromCode(cStr, "comp"))
    {
        mDebugOutput->setText("Done.");
        mParentNode->setShaderCode(mCompiler.getSpirV());

        emit valueChanged();
    }
    else
    {
        mDebugOutput->setText(QString::fromStdString(mCompiler.getError()));
    }
    mDebugOutput->update();
}

void CodeEditorEntity::setParentNode(NodeBase* node)
{
    mParentNode = node;
}

} // namespace Cascade

