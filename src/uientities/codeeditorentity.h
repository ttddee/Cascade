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

#ifndef CODEEDITORENTITY_H
#define CODEEDITORENTITY_H

#include <QWidget>
#include <QSplitter>

#include "../codeeditor/QCodeEditor.hpp"
#include "../shadercompiler/SpvShaderCompiler.h"
#include "uientity.h"

namespace Cascade {

class NodeProperties;

class CodeEditorEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit CodeEditorEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    void setParentNode(NodeBase* node);

private:
    QVBoxLayout* layout;
    QCompleter* completer;
    QStyleSyntaxHighlighter* highlighter;
    QSyntaxStyle* style;
    QSplitter* splitter;
    QCodeEditor* codeEditor;
    QTextEdit* debugOutput;
    SpvCompiler compiler;

    NodeBase* parentNode;


signals:
    void valueChanged();

public slots:
    void handleTextChanged();
};

} // namespace Cascade

#endif // CODEEDITORENTITY_H
