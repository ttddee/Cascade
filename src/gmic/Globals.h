/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file Globals.h
 *
 *  Copyright 2017 Sebastien Fourey
 *
 *  This file is part of G'MIC-Qt, a generic plug-in for raster graphics
 *  editors, offering hundreds of filters thanks to the underlying G'MIC
 *  image processing framework.
 *
 *  gmic_qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gmic_qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gmic_qt.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef GMIC_QT_GLOBALS_H
#define GMIC_QT_GLOBALS_H

#define GMIC_QT_ORGANISATION_NAME "GREYC"
#define GMIC_QT_ORGANISATION_DOMAIN "greyc.fr"
#define GMIC_QT_APPLICATION_NAME "gmic_qt"

namespace GmicQt
{
extern const float PreviewFactorAny;
extern const float PreviewFactorFullImage;
extern const float PreviewFactorActualSize;
} // namespace GmicQt

#define SLIDER_MIN_WIDTH 60
#define PARAMETERS_CACHE_FILENAME "gmic_qt_params.dat"
#define FILTERS_VISIBILITY_FILENAME "gmic_qt_visibility.dat"

#define FAVE_FOLDER_TEXT "<b>Faves</b>"
#define FAVES_IMPORT_KEY "Faves/ImportedGTK179"

#define DARK_THEME_KEY "Config/DarkTheme"
#define REFRESH_USING_INTERNET_KEY "Config/RefreshInternetUpdate"
#define INTERNET_UPDATE_PERIODICITY_KEY "Config/UpdatesPeriodicityValue"
#define INTERNET_NEVER_UPDATE_PERIODICITY std::numeric_limits<int>::max()
#define INTERNET_DEFAULT_PERIODICITY (7 * 24)

#define PREVIEW_MAX_ZOOM_FACTOR 40.0

#define KEYPOINTS_INTERACTIVE_LOWER_DELAY_MS 150
#define KEYPOINTS_INTERACTIVE_UPPER_DELAY_MS 500
#define KEYPOINTS_INTERACTIVE_MIDDLE_DELAY_MS ((KEYPOINTS_INTERACTIVE_LOWER_DELAY_MS + KEYPOINTS_INTERACTIVE_UPPER_DELAY_MS) / 2)
#define KEYPOINTS_INTERACTIVE_AVERAGING_COUNT 6

#endif // GMIC_QT_GLOBALS_H
