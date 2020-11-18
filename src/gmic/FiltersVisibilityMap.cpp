/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file FiltersVisibilityMap.cpp
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
#include "FiltersVisibilityMap.h"
#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include "Common.h"
#include "Globals.h"
//#include "Logger.h"
#include "Utils.h"
//#include "gmic_qt.h"
#include "GmicDefinitions.h"

using namespace Cascade;

QSet<QString> FiltersVisibilityMap::_hiddenFilters;

bool FiltersVisibilityMap::filterIsVisible(const QString & hash)
{
  return !_hiddenFilters.contains(hash);
}

void FiltersVisibilityMap::setVisibility(const QString & hash, bool visible)
{
  if (visible) {
    _hiddenFilters.remove(hash);
  } else {
    _hiddenFilters.insert(hash);
  }
}

void FiltersVisibilityMap::load()
{
  QString path = QString("%1%2").arg(path_rc(false), FILTERS_VISIBILITY_FILENAME);
  QFile file(path);
  if (file.open(QFile::ReadOnly)) {
    QString line;
    do {
      line = file.readLine();
    } while (file.bytesAvailable() && line != QString("[Hidden filters list (compressed)]\n"));
    QByteArray data = qUncompress(file.readAll());
    QBuffer buffer(&data);
    buffer.open(QIODevice::ReadOnly);

    bool ok;
    qint32 count = buffer.readLine().trimmed().toInt(&ok);
    if (ok) {
      QString hash;
      while (count--) {
        hash = buffer.readLine().trimmed();
        _hiddenFilters.insert(hash);
      }
    } else {
      //Logger::error("Cannot read visibility file (" + file.fileName() + ")");
    }
  }
}

void FiltersVisibilityMap::save()
{
  QByteArray data;
  QBuffer buffer(&data);
  buffer.open(QIODevice::WriteOnly);
  qint32 count = _hiddenFilters.size();
  buffer.write(QString("%1\n").arg(count).toLatin1());
  for (const QString & str : _hiddenFilters) {
    buffer.write((str + QChar('\n')).toLatin1());
  }

  QString path = QString("%1%2").arg(path_rc(true), FILTERS_VISIBILITY_FILENAME);
  QFile file(path);
  if (file.open(QFile::WriteOnly)) {
    file.write(QString("Version=%1\n").toLocal8Bit());
    file.write(QString("[Hidden filters list (compressed)]\n").toLocal8Bit());
    file.write(qCompress(data));
    file.close();
  } else {
    //Logger::error("Cannot write " + path);
  }
}
