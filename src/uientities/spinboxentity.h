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

#ifndef SPINBOXENTITY_H
#define SPINBOXENTITY_H

#include <QWidget>

#include "uientity.h"

namespace Ui {
class SpinBoxEntity;
}

namespace Cascade {

class NodeProperties;

class SpinBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SpinBoxEntity(UIElementType et, QWidget *parent = nullptr);

    const QString name() override;
    void setName(const QString& name);

    void setMinMaxStepValue(int, int, int, int);
    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    ~SpinBoxEntity();

private:
    void reset();

    Ui::SpinBoxEntity *mUi;

    int mBaseValue = 0;

signals:
    void valueChanged();
};

} // namespace Cascade

#endif // SPINBOXENTITY_H
