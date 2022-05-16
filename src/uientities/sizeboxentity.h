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

#ifndef SIZEBOXENTITY_H
#define SIZEBOXENTITY_H

#include <QWidget>

#include "uientity.h"

namespace Ui {
class SizeBoxEntity;
}

namespace Cascade {

class NodeProperties;

class SizeBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SizeBoxEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    void loadPropertyValues(const QString& values) override;

    ~SizeBoxEntity();

private:
    void setSizeBoxNoSignal(const QSize& size);

    void hideCustomSizeElements();
    void unHideCustomSizeElements();

    Ui::SizeBoxEntity *mUi;

signals:
    void valueChanged();

public slots:
    void handleSelectionChanged();
};

} // namespace Cascade

#endif // SIZEBOXENTITY_H
