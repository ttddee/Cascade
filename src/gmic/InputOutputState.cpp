/** -*- mode: c++ ; c-basic-offset: 2 -*-
 *
 *  @file InputOutputState.cpp
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

#include "InputOutputState.h"
#include <QJsonObject>

using namespace Cascade::Gmic;


void filterObsoleteInputModes(Gmic::InputMode & mode)
{
  switch (mode) {
  case Gmic::AllDesc_UNUSED:
  case Gmic::AllVisiblesDesc_UNUSED:
  case Gmic::AllInvisiblesDesc_UNUSED:
    mode = Gmic::UnspecifiedInputMode;
    break;
  default:
    break;
  }
}


const InputOutputState InputOutputState::Default(Gmic::DefaultInputMode, Gmic::DefaultOutputMode, Gmic::DefaultPreviewMode);
const InputOutputState InputOutputState::Unspecified(Gmic::UnspecifiedInputMode, Gmic::UnspecifiedOutputMode, Gmic::UnspecifiedPreviewMode);

InputOutputState::InputOutputState() : inputMode(UnspecifiedInputMode), outputMode(UnspecifiedOutputMode), previewMode(Gmic::UnspecifiedPreviewMode) {}

InputOutputState::InputOutputState(InputMode inputMode, OutputMode outputMode, PreviewMode previewMode) : inputMode(inputMode), outputMode(outputMode), previewMode(previewMode) {}

bool InputOutputState::operator==(const InputOutputState & other) const
{
  return inputMode == other.inputMode && outputMode == other.outputMode && previewMode == other.previewMode;
}

bool InputOutputState::operator!=(const InputOutputState & other) const
{
  return inputMode != other.inputMode || outputMode != other.outputMode || previewMode != other.previewMode;
}

bool InputOutputState::isDefault() const
{
  return (inputMode == Gmic::DefaultInputMode) && (outputMode == Gmic::DefaultOutputMode) && (previewMode == Gmic::DefaultPreviewMode);
}

void InputOutputState::toJSONObject(QJsonObject & object) const
{
  object = QJsonObject();
  if (inputMode != UnspecifiedInputMode) {
    object.insert("InputLayers", inputMode);
  }
  if (outputMode != DefaultOutputMode) {
    object.insert("OutputMode", outputMode);
  }
  if (previewMode != DefaultPreviewMode) {
    object.insert("PreviewMode", previewMode);
  }
}

InputOutputState InputOutputState::fromJSONObject(const QJsonObject & object)
{
  Gmic::InputOutputState state;
  state.inputMode = static_cast<InputMode>(object.value("InputLayers").toInt(UnspecifiedInputMode));
  filterObsoleteInputModes(state.inputMode);
  state.outputMode = static_cast<OutputMode>(object.value("OutputMode").toInt(UnspecifiedOutputMode));
  state.previewMode = static_cast<PreviewMode>(object.value("PreviewMode").toInt(UnspecifiedPreviewMode));
  return state;
}

