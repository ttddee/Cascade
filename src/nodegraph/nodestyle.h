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

#pragma once

#include <QtGui/QColor>

#include "style.h"

namespace Cascade::NodeGraph
{

class NodeStyle : public Style
{
public:
    NodeStyle();

    NodeStyle(QString jsonText);

public:
    static void setNodeStyle(QString jsonText);

private:
    void loadJsonText(QString jsonText) override;

    void loadJsonFile(QString fileName) override;

    void loadJsonFromByteArray(QByteArray const& byteArray) override;

public:
    QColor NormalBoundaryColor;
    QColor SelectedBoundaryColor;
    QColor ViewedBoundaryColor;
    QColor GradientColor0;
    QColor GradientColor1;
    QColor GradientColor2;
    QColor GradientColor3;
    QColor ShadowColor;
    QColor FontColor;
    QColor FontColorFaded;

    QColor ConnectionPointColor;
    QColor FilledConnectionPointColor;

    QColor WarningColor;
    QColor ErrorColor;

    float PenWidth;
    float HoveredPenWidth;

    float ConnectionPointDiameter;

    float Opacity;
};
} // namespace Cascade::NodeGraph
