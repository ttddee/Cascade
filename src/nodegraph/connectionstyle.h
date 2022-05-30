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

class ConnectionStyle : public Style
{
public:
    ConnectionStyle();

    ConnectionStyle(QString jsonText);

public:
    static void setConnectionStyle(QString jsonText);

private:
    void loadJsonText(QString jsonText) override;

    void loadJsonFile(QString fileName) override;

    void loadJsonFromByteArray(QByteArray const &byteArray) override;

public:
    QColor constructionColor() const;
    QColor normalColor() const;
    QColor normalColor(const QString& portName) const;
    QColor selectedColor() const;
    QColor selectedHaloColor() const;
    QColor hoveredColor() const;

    float lineWidth() const;
    float constructionLineWidth() const;
    float pointDiameter() const;

private:
    QColor ConstructionColor;
    QColor NormalColor;
    QColor BackColor;
    QColor FrontColor;
    QColor ResultColor;
    QColor SelectedColor;
    QColor SelectedHaloColor;
    QColor HoveredColor;

    float LineWidth;
    float ConstructionLineWidth;
    float PointDiameter;
};
}
