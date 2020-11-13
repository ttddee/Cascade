/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

class NodeProperties;

namespace Ui {
class SizeBoxEntity;
}

class SizeBoxEntity : public UiEntity
{
    Q_OBJECT

public:
    explicit SizeBoxEntity(UIElementType et, QWidget *parent = nullptr);

    void selfConnectToValueChanged(NodeProperties* p);

    QString getValuesAsString() override;

    ~SizeBoxEntity();

private:
    void setSizeBoxNoSignal(const QSize& size);

    void hideCustomSizeElements();
    void unHideCustomSizeElements();

    Ui::SizeBoxEntity *ui;

    QMap<QString, QPair<int, int>> sizePresets =
    {
        { "NTSC", { 720, 486 } },
        { "PAL", { 720, 576 } },
        { "HD 720p", { 1280, 720 } },
        { "HD 1080p", { 1920, 1080 } },
        { "2K", { 2048, 1024 } },
        { "4K", { 4096, 2160 } },
        { "256 Square", { 256, 256 } },
        { "512 Square", { 512, 512 } },
        { "1K Square", { 1024, 1024 } },
        { "2K Square", { 2048, 2048 } }
    };

signals:
    void valueChanged();

public slots:
    void handleSelectionChanged();
};

#endif // SIZEBOXENTITY_H
