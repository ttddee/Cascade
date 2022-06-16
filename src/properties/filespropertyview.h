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

#ifndef FILESPROPERTYVIEW_H
#define FILESPROPERTYVIEW_H

#include <QPushButton>
#include <QListView>

#include "propertyview.h"

namespace Cascade::Properties
{
    class FilesPropertyModel;
}

namespace Cascade::Properties {

using Cascade::Properties::FilesPropertyModel;

class FilesPropertyView : public PropertyView
{
    Q_OBJECT

public:
    FilesPropertyView(QWidget *parent = nullptr);

    void setModel(FilesPropertyModel* model);

private:
    FilesPropertyModel* mModel;

    QVBoxLayout*        mLayout;
    QPushButton*        mLoadButton;
    QListView*          mFileListView;
    QPushButton*        mDeleteButton;
};

} // namespace Cascade::Properties

#endif // FILESPROPERTYVIEW_H
