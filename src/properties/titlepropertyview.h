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

#ifndef TITLEPROPERTYVIEW_H
#define TITLEPROPERTYVIEW_H

#include "propertyview.h"

namespace Cascade::NodeGraph
{
    class TitlePropertyModel;
}

using Cascade::NodeGraph::TitlePropertyModel;

namespace Cascade::Properties {

class TitlePropertyView : public PropertyView
{
    Q_OBJECT

public:
    TitlePropertyView(QWidget *parent = nullptr);

    void setModel(TitlePropertyModel* model);

private:
    TitlePropertyModel* mModel;

    QVBoxLayout* mLayout;
    QLabel* mTitleLabel;
};

} // namespace Cascade::Properties

#endif // TITLEPROPERTYVIEW_H
