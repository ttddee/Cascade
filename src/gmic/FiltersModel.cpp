/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file FiltersModel.cpp
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

#include "FiltersModel.h"

#include <limits>
#include <iostream>

#include <QCryptographicHash>
#include <QDebug>
#include "HtmlTranslator.h"

#include "../log.h"

using namespace Cascade;

const size_t FiltersModel::NoIndex = std::numeric_limits<size_t>::max();

void FiltersModel::printFilterNames()
{
    foreach (Filter f, _hash2filter.values())
    {
        std::cout << f.plainText().toStdString() << std::endl;
        std::cout << f.hash().toStdString() << std::endl;
    }
}

QString FiltersModel::getFilterCommand(const QString& hash)
{
    return _hash2filter[hash].command();
}

QString FiltersModel::getFilterParameters(const QString &hash)
{
    return _hash2filter[hash].parameters();
}

QString FiltersModel::getFilterPath(const QString &hash)
{
    return _hash2filter[hash].path()[0];
}

QString FiltersModel::getFilterCategoryFromHash(const QString &hash)
{
    auto filter = getFilterFromHash(hash);
    return filter.category();
}

void FiltersModel::clear()
{
  _hash2filter.clear();
}

void FiltersModel::addFilter(const FiltersModel::Filter & filter)
{
  if (!(excludedFilters.find(filter.hash()) != excludedFilters.end()))
  {
      _hash2filter[filter.hash()] = filter;
  }

}

void FiltersModel::addFilterCategory(const QString &category)
{
    _filterCategories.insert(category);
}

QSet<QString>& FiltersModel::getFilterCategories()
{
    return _filterCategories;
}

void FiltersModel::flush()
{
  //CS_LOG_INFO() << "Filters\n=======";
  //for (const Filter & filter : (*this)) {
    //CS_LOG_INFO() << "[" << filter.path() << "]" << filter.name();
  //}
}

size_t FiltersModel::filterCount() const
{
  return _hash2filter.size();
}

size_t FiltersModel::notTestingFilterCount() const
{
  const_iterator it = cbegin();
  size_t result = 0;
  while (it != cend()) {
    const QList<QString> & path = it->path();
    if (!path.startsWith("<b>Testing</b>")) {
      ++result;
    }
    ++it;
  }
  return result;
}

const FiltersModel::Filter & FiltersModel::getFilterFromHash(const QString & hash) const
{
  Q_ASSERT_X(_hash2filter.contains(hash), "FiltersModel::getFilterFromHash()", "Hash not found");
  return _hash2filter.find(hash).value();
}

bool FiltersModel::contains(const QString & hash) const
{
  return (_hash2filter.find(hash) != _hash2filter.cend());
}

void FiltersModel::removePath(const QList<QString> & path)
{
  QList<QString> matchingHashes;
  for (const Filter & filter : (*this)) {
    if (filter.matchFullPath(path)) {
      matchingHashes.push_back(filter.hash());
    }
  }
  for (const QString & hash : matchingHashes) {
    _hash2filter.remove(hash);
  }
}

FiltersModel::Filter::Filter()
{
  //_previewFactor = GmicQt::PreviewFactorAny;
  _isAccurateIfZoomed = false;
  _isWarning = false;
}

FiltersModel::Filter & FiltersModel::Filter::setName(const QString & name)
{
  _name = name;
  _plainText = HtmlTranslator::html2txt(name, true);
  _translatedPlainText = HtmlTranslator::html2txt(name);
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setCommand(const QString & command)
{
  _command = command;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setPreviewCommand(const QString & previewCommand)
{
  _previewCommand = previewCommand;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setParameters(const QString & parameters)
{
  _parameters = parameters;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setPreviewFactor(float factor)
{
  _previewFactor = factor;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setAccurateIfZoomed(bool accurate)
{
  _isAccurateIfZoomed = accurate;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setPath(const QList<QString> & path)
{
  _path = path;
  _plainPath.clear();
  _translatedPlainPath.clear();
  foreach (const QString & str, _path) {
    _plainPath.push_back(HtmlTranslator::html2txt(str, true));
    _translatedPlainPath.push_back(HtmlTranslator::html2txt(str, true));
  }
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setWarningFlag(bool flag)
{
  _isWarning = flag;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setDefaultInputMode(Gmic::InputMode mode)
{
  _defaultInputMode = mode;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::setCategory(QString s)
{
  _category = s;
  return *this;
}

FiltersModel::Filter & FiltersModel::Filter::build()
{
  //
  // Caution : This code is duplicated in FavesModel::Fave::build() to
  //           compute the originalHash of a Fave.
  //
  QCryptographicHash hash(QCryptographicHash::Md5);
  hash.addData(_name.toLocal8Bit());
  hash.addData(_command.toLocal8Bit());
  hash.addData(_previewCommand.toLocal8Bit());
  _hash = hash.result().toHex();
  return *this;
}

const QString & FiltersModel::Filter::name() const
{
  return _name;
}

const QString & FiltersModel::Filter::plainText() const
{
  return _plainText;
}

const QString & FiltersModel::Filter::translatedPlainText() const
{
  return _translatedPlainText;
}

const QList<QString> & FiltersModel::Filter::path() const
{
  return _path;
}

const QString & FiltersModel::Filter::hash() const
{
  return _hash;
}

QString FiltersModel::Filter::hash236() const
{
  QCryptographicHash hash(QCryptographicHash::Md5);
  QString lowerName(_name);
  //GmicQt::downcaseCommandTitle(lowerName);
  hash.addData(lowerName.toLocal8Bit());
  hash.addData(_command.toLocal8Bit());
  hash.addData(_previewCommand.toLocal8Bit());
  return hash.result().toHex();
}

const QString & FiltersModel::Filter::command() const
{
  return _command;
}

const QString & FiltersModel::Filter::previewCommand() const
{
  return _previewCommand;
}

const QString & FiltersModel::Filter::parameters() const
{
  return _parameters;
}

float FiltersModel::Filter::previewFactor() const
{
  return _previewFactor;
}

bool FiltersModel::Filter::isAccurateIfZoomed() const
{
  return _isAccurateIfZoomed;
}

bool FiltersModel::Filter::isWarning() const
{
  return _isWarning;
}

Gmic::InputMode FiltersModel::Filter::defaultInputMode() const
{
  return _defaultInputMode;
}

const QString & FiltersModel::Filter::category() const
{
  return _category;
}

bool FiltersModel::Filter::matchKeywords(const QList<QString> & keywords) const
{
  QList<QString>::const_iterator itKeyword = keywords.cbegin();
  while (itKeyword != keywords.cend()) {
    // Check that this keyword is present, either in filter name or in its path
    const QString & keyword = *itKeyword;
    bool keywordInPath = false;
    QList<QString>::const_iterator itPath = _translatedPlainPath.cbegin();
    while (itPath != _translatedPlainPath.cend() && !keywordInPath) {
      keywordInPath = itPath->contains(keyword, Qt::CaseInsensitive);
      ++itPath;
    }
    if (!keywordInPath && !_translatedPlainText.contains(keyword, Qt::CaseInsensitive)) {
      return false;
    }
    ++itKeyword;
  }
  return true;
}

bool FiltersModel::Filter::matchFullPath(const QList<QString> & pathToMatch) const
{
  QList<QString>::const_iterator it = _plainPath.cbegin();
  QList<QString>::const_iterator itToMatch = pathToMatch.cbegin();
  while ((it != _plainPath.cend()) && (itToMatch != pathToMatch.cend()) && (*it == *itToMatch)) {
    ++it;
    ++itToMatch;
  }
  return (itToMatch == pathToMatch.cend()) || ((it == _plainPath.cend()) && (itToMatch != pathToMatch.cend()) && (_plainText == *itToMatch));
}

FiltersModel::const_iterator::const_iterator(const QMap<QString, Filter>::const_iterator & iterator)
{
  _mapIterator = iterator;
}

const FiltersModel::Filter & FiltersModel::const_iterator::operator*() const
{
  return _mapIterator.value();
}

FiltersModel::const_iterator & FiltersModel::const_iterator::operator++()
{
  ++_mapIterator;
  return *this;
}

FiltersModel::const_iterator FiltersModel::const_iterator::operator++(int)
{
  FiltersModel::const_iterator current(*this);
  ++(*this);
  return current;
}

const FiltersModel::Filter * FiltersModel::const_iterator::operator->() const
{
  return &(_mapIterator.value());
}

bool FiltersModel::const_iterator::operator!=(const FiltersModel::const_iterator & other) const
{
  return _mapIterator != other._mapIterator;
}

bool FiltersModel::const_iterator::operator==(const FiltersModel::const_iterator & other) const
{
  return _mapIterator == other._mapIterator;
}
