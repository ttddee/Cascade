/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file FiltersModel.h
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

#ifndef GMIC_QT_FILTERSMODEL_H
#define GMIC_QT_FILTERSMODEL_H
#include <QList>
#include <QMap>
#include <QString>
#include <cstddef>
#include <vector>
//#include "gmic_qt.h"
#include "GmicDefinitions.h"

using namespace Cascade;

class FiltersModel {
public:
  class Filter {
  public:
    Filter();
    Filter & setName(const QString & name);
    Filter & setCommand(const QString & command);
    Filter & setPreviewCommand(const QString & previewCommand);
    Filter & setParameters(const QString & parameters);
    Filter & setPreviewFactor(float factor);
    Filter & setAccurateIfZoomed(bool accurate);
    Filter & setPath(const QList<QString> & path);
    Filter & setWarningFlag(bool flag);
    Filter & setDefaultInputMode(Gmic::InputMode);
    Filter & build();

    const QString & name() const;
    const QString & plainText() const;
    const QString & translatedPlainText() const;
    const QList<QString> & path() const;
    const QString & hash() const;
    QString hash236() const;
    const QString & command() const;
    const QString & previewCommand() const;
    const QString & parameters() const;
    float previewFactor() const;
    bool isAccurateIfZoomed() const;
    bool isWarning() const;
    Gmic::InputMode defaultInputMode() const;

    bool matchKeywords(const QList<QString> & keywords) const;
    bool matchFullPath(const QList<QString> & path) const;

  private:
    QString _name;
    QString _plainText;
    QString _translatedPlainText;
    QList<QString> _path;
    QList<QString> _plainPath;
    QList<QString> _translatedPlainPath;
    QString _command;
    QString _previewCommand;
    Gmic::InputMode _defaultInputMode;
    QString _parameters;
    float _previewFactor;
    bool _isAccurateIfZoomed;
    QString _hash;
    bool _isWarning;
  };

  FiltersModel() = default;
  ~FiltersModel() = default;

public:
  void printFilterNames();
  void clear();
  void addFilter(const Filter & filter);
  void flush();
  size_t filterCount() const;
  size_t notTestingFilterCount() const;
  const Filter & getFilterFromHash(const QString & hash) const;
  bool contains(const QString & hash) const;
  static const size_t NoIndex;

  void removePath(const QList<QString> & path);

  class const_iterator {
  public:
    const_iterator(const QMap<QString, Filter>::const_iterator & iterator);
    const Filter & operator*() const;
    const_iterator & operator++();
    const_iterator operator++(int);
    const Filter * operator->() const;
    bool operator!=(const FiltersModel::const_iterator & other) const;
    bool operator==(const FiltersModel::const_iterator & other) const;

  private:
    QMap<QString, Filter>::const_iterator _mapIterator;
  };

  const_iterator begin() const { return _hash2filter.cbegin(); }
  const_iterator end() const { return _hash2filter.cend(); }
  const_iterator cbegin() const { return _hash2filter.cbegin(); }
  const_iterator cend() const { return _hash2filter.cend(); }

private:
  QMap<QString, Filter> _hash2filter;
};

#endif // GMIC_QT_FILTERSMODEL_H
