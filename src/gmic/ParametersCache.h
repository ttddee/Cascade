/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file ParametersCache.h
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
#ifndef GMIC_QT_PARAMETERSCACHE_H
#define GMIC_QT_PARAMETERSCACHE_H

#include <QHash>
#include <QList>
#include <QString>
#include "InputOutputState.h"

using namespace Cascade;

class ParametersCache {
public:
  static void load(bool loadFiltersParameters);
  static void save();
  static void setValues(const QString & hash, const QList<QString> & values);
  static QList<QString> getValues(const QString & hash);
  static void setVisibilityStates(const QString & hash, const QList<int> & states);
  static QList<int> getVisibilityStates(const QString & hash);
  static void remove(const QString & hash);

  static Gmic::InputOutputState getInputOutputState(const QString & hash);
  static void setInputOutputState(const QString & hash, const Gmic::InputOutputState & state, const Gmic::InputMode defaultInputMode);

  static void cleanup(const QSet<QString> & hashesToKeep);

private:
  static QHash<QString, QList<QString>> _parametersCache;
  static QHash<QString, Gmic::InputOutputState> _inOutPanelStates;
  static QHash<QString, QList<int>> _visibilityStates;
};

#endif // GMIC_QT_PARAMETERSCACHE_H
