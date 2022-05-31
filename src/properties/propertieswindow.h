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

#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include <QWidget>

#include "propertywidget.h"
#include "../nodegraph/node.h"

using Cascade::NodeGraph::Node;

namespace Cascade::Properties
{

class PropertiesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesWindow(QWidget *parent = nullptr);

private:
    void setPropertyWidget(PropertyWidget* widget);
    void clear();

    QVBoxLayout* mLayout;
    PropertyWidget* mPropertyWidget = nullptr;

public slots:
    void handleActiveNodeChanged(Cascade::NodeGraph::Node* node);
};

} // namespace Cascade::Properties

#endif // PROPERTIESWINDOW_H
