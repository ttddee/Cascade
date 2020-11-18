/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file ParametersCache.cpp
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
#include "ParametersCache.h"
#include <QBuffer>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include "Common.h"
#include "Globals.h"
#include "Utils.h"
#include "gmic.h"

QHash<QString, QList<QString>> ParametersCache::_parametersCache;
QHash<QString, Gmic::InputOutputState> ParametersCache::_inOutPanelStates;
QHash<QString, QList<int>> ParametersCache::_visibilityStates;

void ParametersCache::load(bool loadFiltersParameters)
{
  // Load JSON file
  _parametersCache.clear();
  _inOutPanelStates.clear();
  _visibilityStates.clear();

  QString jsonFilename = QString("%1%2").arg(path_rc(true), PARAMETERS_CACHE_FILENAME);
  QFile jsonFile(jsonFilename);
  if (!jsonFile.exists()) {
    return;
  }
  if (jsonFile.open(QFile::ReadOnly)) {
#ifdef _GMIC_QT_DEBUG_
    QJsonDocument jsonDoc;
    QByteArray allFile = jsonFile.readAll();
    if (allFile.startsWith("{")) {
      jsonDoc = QJsonDocument::fromJson(allFile);
    } else {
      jsonDoc = QJsonDocument::fromJson(qUncompress(allFile));
    }
#else
    QJsonDocument jsonDoc = QJsonDocument::fromJson(qUncompress(jsonFile.readAll()));
#endif
    if (jsonDoc.isNull()) {
      //Logger::warning(QString("Cannot parse ") + jsonFilename);
      //Logger::warning("Last filters parameters are lost!");
    } else {
      if (!jsonDoc.isObject()) {
        //Logger::error(QString("JSON file format is not correct (") + jsonFilename + ")");
      } else {
        QJsonObject documentObject = jsonDoc.object();
        QJsonObject::iterator itFilter = documentObject.begin();
        while (itFilter != documentObject.end()) {
          QString hash = itFilter.key();
          QJsonObject filterObject = itFilter.value().toObject();
          // Retrieve parameters
          if (loadFiltersParameters) {
            QJsonValue parameters = filterObject.value("parameters");
            if (!parameters.isUndefined()) {
              QJsonArray array = parameters.toArray();
              QStringList values;
              for (const QJsonValueRef & v : array) {
                values.push_back(v.toString());
              }
              _parametersCache[hash] = values;
            }
            QJsonValue visibilityStates = filterObject.value("visibility_states");
            if (!visibilityStates.isUndefined()) {
              QJsonArray array = visibilityStates.toArray();
              QList<int> values;
              for (const QJsonValueRef & v : array) {
                values.push_back(v.toInt());
              }
              _visibilityStates[hash] = values;
            }
          }
          QJsonValue state = filterObject.value("in_out_state");
          // Retrieve Input/Output state
          if (!state.isUndefined()) {
            QJsonObject stateObject = state.toObject();
            _inOutPanelStates[hash] = Gmic::InputOutputState::fromJSONObject(stateObject);
          }
          ++itFilter;
        }
      }
    }
  } else {
    //Logger::error("Cannot read " + jsonFilename);
    //Logger::error("Parameters cannot be restored");
  }
}

void ParametersCache::save()
{
  // JSON Document format
  //
  // {
  //  "51d288e6f1c6e531cc61289f17e34d8a": {
  //      "parameters": [
  //          "6",
  //          "21.06",
  //          "1.36",
  //          "5",
  //          "0"
  //      ],
  //      "in_out_state": {
  //          "InputLayers": 1,
  //          "OutputMessages": 5,
  //          "OutputMode": 100,
  //          "PreviewMode": 100
  //      }
  //      "visibility_states": [
  //             0,
  //             1,
  //             2,
  //             0
  //      ]
  //  }
  // }

  QJsonObject documentObject;

  // Add Input/Output states
  QHash<QString, Gmic::InputOutputState>::iterator itState = _inOutPanelStates.begin();
  while (itState != _inOutPanelStates.end()) {
    QJsonObject filterObject;
    QJsonObject jsonState;
    itState.value().toJSONObject(jsonState);
    filterObject.insert("in_out_state", jsonState);
    documentObject.insert(itState.key(), filterObject);
    ++itState;
  }

  // Add filters parameters

  QHash<QString, QList<QString>>::iterator itParams = _parametersCache.begin();
  while (itParams != _parametersCache.end()) {
    QJsonObject filterObject;
    QJsonObject::iterator entry = documentObject.find(itParams.key());
    if (entry != documentObject.end()) {
      filterObject = entry.value().toObject();
    }
    // Add the parameters list
    QJsonArray array;
    QStringList list = itParams.value();
    for (const QString & str : list) {
      array.push_back(str);
    }
    filterObject.insert("parameters", array);
    documentObject.insert(itParams.key(), filterObject);
    ++itParams;
  }

  // Add visibility states

  QHash<QString, QList<int>>::iterator itVisibilities = _visibilityStates.begin();
  while (itVisibilities != _visibilityStates.end()) {
    QJsonObject filterObject;
    QJsonObject::iterator entry = documentObject.find(itVisibilities.key());
    if (entry != documentObject.end()) {
      filterObject = entry.value().toObject();
    }
    // Add the parameters list
    QJsonArray array;
    QList<int> states = itVisibilities.value();
    for (const int & state : states) {
      array.push_back(state);
    }
    filterObject.insert("visibility_states", array);
    documentObject.insert(itVisibilities.key(), filterObject);
    ++itVisibilities;
  }

  QJsonDocument jsonDoc(documentObject);
  QString jsonFilename = QString("%1%2").arg(path_rc(true), PARAMETERS_CACHE_FILENAME);
  QFile jsonFile(jsonFilename);
  if (QFile::exists(jsonFilename)) {
    QString bakFilename = QString("%1%2").arg(path_rc(false), PARAMETERS_CACHE_FILENAME ".bak");
    QFile::remove(bakFilename);
    QFile::copy(jsonFilename, bakFilename);
  }
  if (jsonFile.open(QFile::WriteOnly | QFile::Truncate)) {
#ifdef _GMIC_QT_DEBUG_
    qint64 count = jsonFile.write(jsonDoc.toJson());
#else
    qint64 count = jsonFile.write(qCompress(jsonDoc.toJson(QJsonDocument::Compact)));
#endif
    // jsonFile.write(jsonDoc.toJson());
    // jsonFile.write(qCompress(jsonDoc.toBinaryData()));
    jsonFile.close();
    if (count != -1) {
      // Remove obsolete 2.0.0 pre-release files
      const QString & path = path_rc(true);
      QFile::remove(path + "gmic_qt_parameters.dat");
      QFile::remove(path + "gmic_qt_parameters.json");
      QFile::remove(path + "gmic_qt_parameters.json.bak");
      QFile::remove(path + "gmic_qt_parameters_json.dat");
    }
  } else {
    //Logger::error("Cannot write " + jsonFilename);
    //Logger::error("Parameters cannot be saved");
  }
}

void ParametersCache::setValues(const QString & hash, const QList<QString> & values)
{
  _parametersCache[hash] = values;
}

QList<QString> ParametersCache::getValues(const QString & hash)
{
  if (_parametersCache.contains(hash)) {
    return _parametersCache[hash];
  }
  return QList<QString>();
}

void ParametersCache::setVisibilityStates(const QString & hash, const QList<int> & states)
{
  _visibilityStates[hash] = states;
}

QList<int> ParametersCache::getVisibilityStates(const QString & hash)
{
  if (_visibilityStates.contains(hash)) {
    return _visibilityStates[hash];
  }
  return QList<int>();
}

void ParametersCache::remove(const QString & hash)
{
  _parametersCache.remove(hash);
  _inOutPanelStates.remove(hash);
}

Gmic::InputOutputState ParametersCache::getInputOutputState(const QString & hash)
{
  if (_inOutPanelStates.contains(hash)) {
    return _inOutPanelStates[hash];
  }
  return Gmic::InputOutputState(Gmic::UnspecifiedInputMode, Gmic::DefaultOutputMode, Gmic::DefaultPreviewMode);
}

void ParametersCache::setInputOutputState(const QString & hash, const Gmic::InputOutputState & state, const Gmic::InputMode defaultInputMode)
{
  if ((state == Gmic::InputOutputState(defaultInputMode, Gmic::DefaultOutputMode, Gmic::DefaultPreviewMode)) //
      || (state == Gmic::InputOutputState(Gmic::UnspecifiedInputMode, Gmic::DefaultOutputMode, Gmic::DefaultPreviewMode))) {
    _inOutPanelStates.remove(hash);
    return;
  }
  _inOutPanelStates[hash] = state;
}

void ParametersCache::cleanup(const QSet<QString> & hashesToKeep)
{
  QSet<QString> obsoleteHashes;

  // Build set of no longer used parameters
  QHash<QString, QList<QString>>::iterator itParam = _parametersCache.begin();
  while (itParam != _parametersCache.end()) {
    if (!hashesToKeep.contains(itParam.key())) {
      obsoleteHashes.insert(itParam.key());
    }
    ++itParam;
  }
  for (const QString & h : obsoleteHashes) {
    _parametersCache.remove(h);
  }
  obsoleteHashes.clear();

  // Build set of no longer used In/Out states
  QHash<QString, Gmic::InputOutputState>::iterator itState = _inOutPanelStates.begin();
  while (itState != _inOutPanelStates.end()) {
    if (!hashesToKeep.contains(itState.key())) {
      obsoleteHashes.insert(itState.key());
    }
    ++itState;
  }
  for (const QString & h : obsoleteHashes) {
    _inOutPanelStates.remove(h);
  }
  obsoleteHashes.clear();
}
