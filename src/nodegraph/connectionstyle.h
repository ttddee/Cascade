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
