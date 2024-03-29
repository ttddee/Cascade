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

#ifndef UIENTITY_H
#define UIENTITY_H

#include <QObject>
#include <QWidget>

//#include "../nodegraph/nodedefinitions.h"
//#include "../nodegraph/nodeproperties.h"
#include "../log.h"

namespace Cascade {

enum class UIElementType
{
    ePropertiesHeading,
    eSpinBox,
    eFileBox,
    eColorButton,
    eWriteProperties,
    eComboBox,
    eChannelSelect,
    eSliderBoxDouble,
    eSliderBoxInt,
    eColorProperties,
    eSizeBox,
    eTextBox,
    eCheckBox,
    eTextBrowser, // TODO: This is not used anywhere
    eSeparator,
    eLineEdit, // TODO: This is not used anywhere
    eFolderBox, // TODO: This is not used anywhere
    eResizeProperties,
    eCodeEditor
};

class UiEntity : public QWidget
{
    Q_OBJECT

public:
    explicit UiEntity(UIElementType et, QWidget *parent = nullptr);

    virtual QString getValuesAsString() = 0;

    virtual void loadPropertyValues(const QString& values) = 0;

    virtual const QString name();

    const UIElementType mElementType;
};

} // namespace Cascade

#endif // UIENTITY_H
