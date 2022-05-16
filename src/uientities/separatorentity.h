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

#ifndef SEPARATORENTITY_H
#define SEPARATORENTITY_H

#include <QWidget>

#include "uientity.h"

namespace Ui {
class SeparatorEntity;
}

namespace Cascade {

class NodeProperties;

class SeparatorEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SeparatorEntity(UIElementType et, QWidget *parent = nullptr);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    ~SeparatorEntity();

private:
    Ui::SeparatorEntity *mUi;
};

} // namespace Cascade

#endif // SEPARATORENTITY_H
