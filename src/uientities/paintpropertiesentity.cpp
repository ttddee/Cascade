#include "paintpropertiesentity.h"

PaintPropertiesEntity::PaintPropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent)
{

}

QString PaintPropertiesEntity::getValuesAsString()
{
    QString s;
    s.append(QString::number(viewerClickPos.x()));
    s.append(",");
    s.append(QString::number(viewerClickPos.y()));

    return s;
}

void PaintPropertiesEntity::loadPropertyValues(const QString &values)
{
    // TODO
//    auto split = values.split(",");
//    QColor c(split[0].toInt(), split[1].toInt(), split[2].toInt(), split[3].toInt());
//    setColor(c);
}

void PaintPropertiesEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &PaintPropertiesEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

void PaintPropertiesEntity::handleViewerClick(QPoint pos)
{
    viewerClickPos = pos;

    emit valueChanged();
}


