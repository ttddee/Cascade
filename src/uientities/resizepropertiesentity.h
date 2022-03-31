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

#ifndef RESIZEPROPERTIESENTITY_H
#define RESIZEPROPERTIESENTITY_H

#include <QWidget>
#include <QSpinBox>

#include "uientity.h"

#include <gtest/gtest_prod.h>

namespace Ui {
class ResizePropertiesEntity;
}

namespace Cascade {

class NodeProperties;
class NodeBase;

class ResizePropertiesEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit ResizePropertiesEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    void setParentNode(NodeBase* node);

    ~ResizePropertiesEntity();

private:
    FRIEND_TEST(ResizePropertiesEntityTest, getValuesAsString_CorrectInitialValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setWidthPercentLinked_CorrectPixelValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setHeightPercentLinked_CorrectPixelValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setWidthPercenNotLinked_CorrectPixelValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setHeightPercenNotLinked_CorrectPixelValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setWidthPixelsLinked_CorrectPercentValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setHeightPixelsLinked_CorrectPercentValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setWidthPixelsNotLinked_CorrectPercentValues);
    FRIEND_TEST(ResizePropertiesEntityTest, setHeightPixelsNotLinked_CorrectPercentValues);

    void widthPercentChanged();
    void heightPercentChanged();
    void widthPixelsChanged();
    void heightPixelsChanged();

    void setInputSize(const QSize& s);
    void setLinked(bool b);

    double getWidthPercent() const;
    double getHeightPercent() const;

    void setWidthPercent(const double d);
    void setHeightPercent(const double d);
    void setWidthPixels(const int i);
    void setHeightPixels(const int i);

    void updatePercentWidth();
    void updatePercentHeight();
    void updatePixelWidth();
    void updatePixelHeight();

    void setSpinBoxNoSignal(QSpinBox* box, int i);
    void setDoubleSpinBoxNoSignal(QDoubleSpinBox* box, double d);

    Ui::ResizePropertiesEntity *ui;

    NodeBase* parentNode;
    QSize inputSize = QSize(0, 0);

    double factorX = 1.0;
    double factorY = 1.0;

signals:
    void valueChanged();

public slots:
    void handleNodeRequestUpdate();
};

} // namespace Cascade

#endif // RESIZEPROPERTIESENTITY_H
