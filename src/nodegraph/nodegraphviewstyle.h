#pragma once

#include <QtGui/QColor>

#include "style.h"

namespace Cascade::NodeGraph
{

class NodeGraphViewStyle : public Style
{
public:
    NodeGraphViewStyle();

    NodeGraphViewStyle(QString jsonText);

public:
    static void setStyle(QString jsonText);

private:
    void loadJsonText(QString jsonText) override;

    void loadJsonFile(QString fileName) override;

    void loadJsonFromByteArray(QByteArray const &byteArray) override;

public:
    QColor BackgroundColor;
    QColor FineGridColor;
    QColor CoarseGridColor;
};
}
