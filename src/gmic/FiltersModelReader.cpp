/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file FiltersModelReader.cpp
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
#include "FiltersModelReader.h"

#include <QBuffer>
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QLocale>
#include <QRegularExpression>
#include <QSettings>
#include <QString>
#include <QRegExp>

#include "FiltersModel.h"
#include "Globals.h"
#include "Utils.h"
#include "gmic.h"

FiltersModelReader::FiltersModelReader(FiltersModel & model) : _model(model) {}

void FiltersModelReader::parseFiltersDefinitions(QByteArray & stdlibArray)
{
   qDebug("Gmic: Starting to load filters.");

  QBuffer stdlib(&stdlibArray);
  stdlib.open(QBuffer::ReadOnly | QBuffer::Text);
  QList<QString> filterPath;

  QString language = "void";

  // Use _en locale if no localization for the language is found.
  QByteArray localePrefix = QString("#@gui_%1").arg(language).toLocal8Bit();
  if (!textIsPrecededBySpacesInSomeLineOfArray(localePrefix, stdlibArray)) {
    language = "en";
  }

  QString buffer = readBufferLine(stdlib);
  QString line;

  QRegExp folderRegexpNoLanguage("^\\s*#@gui[ ][^:]+$");
  QRegExp folderRegexpLanguage(QString("^\\s*#@gui_%1[ ][^:]+$").arg(language));

  QRegExp filterRegexpNoLanguage("^\\s*#@gui[ ][^:]+[ ]*:.*");
  QRegExp filterRegexpLanguage(QString("^\\s*#@gui_%1[ ][^:]+[ ]*:.*").arg(language));

  QRegExp hideCommandRegExp(QString("^\\s*#@gui_%1[ ]+hide\\((.*)\\)").arg(language));
  QRegExp guiComment("^\\s*#@gui");
  QVector<QString> hiddenPaths;

  const QChar WarningPrefix('!');
  do {
    line = buffer.trimmed();
    if (guiComment.indexIn(line) == 0) {
      if (hideCommandRegExp.exactMatch(line)) {
        QString path = hideCommandRegExp.cap(1);
        hiddenPaths.push_back(path);
        buffer = readBufferLine(stdlib);
      } else if (folderRegexpNoLanguage.exactMatch(line) || folderRegexpLanguage.exactMatch(line)) {
        //
        // A folder
        //
        QString folderName = line;
        folderName.replace(QRegExp("^\\s*#@gui[_a-zA-Z]{0,3}[ ]"), "");

        while (folderName.startsWith("_") && !filterPath.isEmpty()) {
          folderName.remove(0, 1);
          filterPath.pop_back();
        }
        while (folderName.startsWith("_")) {
          folderName.remove(0, 1);
        }
        if (!folderName.isEmpty()) {
          filterPath.push_back(folderName);
        }
        buffer = readBufferLine(stdlib);
      } else if (filterRegexpNoLanguage.exactMatch(line) || filterRegexpLanguage.exactMatch(line)) {
        //
        // A filter
        //
        QString filterName = line;
        filterName.replace(QRegExp("[ ]*:.*$"), "");
        filterName.replace(QRegExp("^\\s*#@gui[_a-zA-Z]{0,3}[ ]"), "");
        const bool warning = filterName.startsWith(WarningPrefix);
        if (warning) {
          filterName.remove(0, 1);
        }

        QString filterCommands = line;
        filterCommands.replace(QRegExp("^\\s*#@gui[_a-zA-Z]{0,3}[ ][^:]+[ ]*:[ ]*"), "");

        // Extract default input mode
        Gmic::InputMode defaultInputMode = Gmic::UnspecifiedInputMode;
        QRegExp reInputMode("\\s*:\\s*([xX.*+vViI-])\\s*$");
        if (reInputMode.indexIn(filterCommands) != -1) {
          QString mode = reInputMode.cap(1);
          filterCommands.remove(reInputMode);
          defaultInputMode = symbolToInputMode(mode);
        }

        QList<QString> commands = filterCommands.split(",");
        QString filterCommand = commands[0].trimmed();
        if (commands.isEmpty()) {
          commands.push_back("_none_");
        }
        if (commands.size() == 1) {
          commands.push_back(commands.front());
        }
        QList<QString> preview = commands[1].trimmed().split("(");
        float previewFactor = GmicQt::PreviewFactorAny;
        bool accurateIfZoomed = true;
        if (preview.size() >= 2) {
          if (preview[1].endsWith("+")) {
            accurateIfZoomed = true;
            preview[1].chop(1);
          } else {
            accurateIfZoomed = false;
          }
          previewFactor = preview[1].replace(QRegExp("\\).*"), "").toFloat();
        }
        QString filterPreviewCommand = preview[0].trimmed();

        QString start = line;
        start.replace(QRegExp("^\\s*"), "");
        start.replace(QRegExp(" .*"), " :");
        QRegExp startRegexp(QString("^\\s*%1").arg(start));

        // Read parameters
        QString parameters;
        do {
          buffer = readBufferLine(stdlib);
          if (startRegexp.indexIn(buffer) == 0) {
            QString parameterLine = buffer;
            parameterLine.replace(QRegExp("^\\s*#@gui[_a-zA-Z]{0,3}[ ]*:[ ]*"), "");
            parameters += parameterLine;
          }
        } while (!stdlib.atEnd()                               //
                 && !folderRegexpNoLanguage.exactMatch(buffer) //
                 && !folderRegexpLanguage.exactMatch(buffer)   //
                 && !filterRegexpNoLanguage.exactMatch(buffer) //
                 && !filterRegexpLanguage.exactMatch(buffer));

        QString category = filterPath[0].replace(QRegExp("<[^>]*>"), "");
        category.replace(QRegExp("&"), "and");
        filterName.replace(QRegExp("&"), "and");

        FiltersModel::Filter filter;
        filter.setName(filterName);
        filter.setCommand(filterCommand);
        filter.setPreviewCommand(filterPreviewCommand);
        filter.setDefaultInputMode(defaultInputMode);
        filter.setPreviewFactor(previewFactor);
        filter.setAccurateIfZoomed(accurateIfZoomed);
        filter.setParameters(parameters);
        filter.setPath(filterPath);
        filter.setWarningFlag(warning);
        filter.setCategory(category);
        filter.build();

        _model.addFilter(filter);

        if (category != "About")
            _model.addFilterCategory(category);
      } else {
        buffer = readBufferLine(stdlib);
      }
    } else {
      buffer = readBufferLine(stdlib);
    }
  } while (!buffer.isEmpty());

  // Remove hidden filters from the model
  for (const QString & path : hiddenPaths) {
    const size_t count = _model.filterCount();
    QList<QString> pathList = path.split("/");
    _model.removePath(pathList);
    if (_model.filterCount() == count) {
      qDebug("While hiding filter, name or path not found.");
    }
  }
  qDebug("Gmic: Loaded filters.");
}

bool FiltersModelReader::textIsPrecededBySpacesInSomeLineOfArray(const QByteArray & text, const QByteArray & array)
{
  if (text.isEmpty()) {
    return false;
  }
  int from = 0;
  int position;
  const char * data = array.constData();
  while ((position = array.indexOf(text, from)) != -1) {
    int index = position - 1;
    while ((index >= 0) && (data[index] != '\n') && (data[index] <= ' ')) {
      --index;
    }
    if ((index < 0) || (data[index] == '\n')) {
      return true;
    }
    from = position + 1;
  }
  return false;
}

Gmic::InputMode FiltersModelReader::symbolToInputMode(const QString & str)
{
  if (str.length() != 1) {
    //Logger::warning(QString("'%1' is not recognized as a default input mode (should be a single symbol/letter)").arg(str));
    return Gmic::UnspecifiedInputMode;
  }
  switch (str.toLocal8Bit()[0]) {
  case 'x':
  case 'X':
    return Gmic::NoInput;
  case '.':
    return Gmic::Active;
  case '*':
    return Gmic::All;
  case '-':
    return Gmic::ActiveAndAbove;
  case '+':
    return Gmic::ActiveAndBelow;
  case 'V':
  case 'v':
    return Gmic::AllVisible;
  case 'I':
  case 'i':
    return Gmic::AllInvisible;
  default:
    //Logger::warning(QString("'%1' is not recognized as a default input mode").arg(str));
    return Gmic::UnspecifiedInputMode;
  }
}

QString FiltersModelReader::readBufferLine(QBuffer & buffer)
{
  // QBuffer::readline(max_size) may be very slow, in debug mode, when max_size
  // is too big (e.g. 1MB). We read large lines in multiple calls.
  QString result;
  QString text;
  QRegExp commentStart("^\\s*#");
  do {
    text = buffer.readLine(1024);
    result.append(text);
  } while (!text.isEmpty() && !text.endsWith("\n"));

  // Merge comment lines ending with '\'
  if (commentStart.indexIn(result) == 0) {
    while (result.endsWith("\\\n")) {
      QString nextLinePeek = buffer.peek(1024);
      if (commentStart.indexIn(nextLinePeek) == -1) {
        return result;
      }
      const QString nextCommentPrefix = commentStart.cap(0);
      result.chop(2);
      QString nextLine;
      do {
        text = buffer.readLine(1024);
        nextLine.append(text);
      } while (!text.isEmpty() && !text.endsWith("\n"));
      int ignoreCount = nextCommentPrefix.length();
      const int limit = nextLine.length() - nextLine.endsWith("\n");
      while (ignoreCount < limit && nextLine[ignoreCount] <= ' ') {
        ++ignoreCount;
      }
      result.append(nextLine.rightRef(nextLine.length() - ignoreCount));
    }
  }
  return result;
}
