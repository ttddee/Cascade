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

#ifndef VIEWERSTATUSBAR_H
#define VIEWERSTATUSBAR_H

#include <QWidget>

namespace Ui {
class ViewerStatusBar;
}

class ViewerStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerStatusBar(QWidget *parent = nullptr);

    void setZoomText(const QString& s);
    void setWidthText(const QString& s);
    void setHeightText(const QString& s);
    void setViewerModeText(const QString& s);

    ~ViewerStatusBar();

private:
    Ui::ViewerStatusBar *ui;

signals:
    void requestZoomReset();
};

#endif // VIEWERSTATUSBAR_H
