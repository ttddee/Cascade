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

#ifndef VIEWERSTATUSBAR_H
#define VIEWERSTATUSBAR_H

#include <QWidget>

#include "uientities/cssliderboxentity.h"
#include "global.h"

namespace Ui {
class ViewerStatusBar;
}

namespace Cascade {

class ViewerStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerStatusBar(QWidget *parent = nullptr);

    void setZoomText(const QString& s);
    void setWidthText(const QString& s);
    void setHeightText(const QString& s);
    void setViewerMode(const ViewerMode m);

    QString getViewerSettings();

    ~ViewerStatusBar();

private:
    Ui::ViewerStatusBar *ui;

    bool split = false;
    bool bw = false;
    ViewerMode currentViewerMode = VIEWER_MODE_OUTPUT_RGB;

    CsSliderBoxEntity* splitSlider;
    CsSliderBoxEntity* gammaSlider;
    CsSliderBoxEntity* gainSlider;

signals:
    void requestZoomReset();
    void valueChanged();
    void viewerModeChanged(const Cascade::ViewerMode mode);

public slots:
    void handleSplitToggled();
    void handleSplitSliderChanged();
    void handleBwToggled();
    void handleValueChanged();
    void handleViewerModeCheckBoxChanged();
};

} // namespace Cascade

#endif // VIEWERSTATUSBAR_H
