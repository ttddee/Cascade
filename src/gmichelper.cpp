#include "gmichelper.h"

#include <iostream>

#include "gmic/GmicStdlib.h"
#include "gmic/FiltersModelReader.h"
#include "gmic/ParametersCache.h"
#include "gmic.h"

GmicHelper& GmicHelper::getInstance()
{
    static GmicHelper instance;

    return instance;
}

void GmicHelper::setUp()
{
    readFilters();

    gmicInstance = std::shared_ptr<gmic>(new gmic(
                                          QString("").toLocal8Bit().constData(),
                                          GmicStdLib::Array.constData(),
                                          true,
                                          0, 0, 0.f));


    //filtersModel.printFilterNames();

}

std::shared_ptr<gmic> GmicHelper::getGmicInstance()
{
    return gmicInstance;
}

QSet<QString>& GmicHelper::getFilterCategories()
{
    return filtersModel.getFilterCategories();
}

FiltersModel& GmicHelper::getFiltersModel()
{
    return filtersModel;
}

void GmicHelper::readFilters()
{
  filtersModel.clear();
  if (GmicStdLib::Array.isEmpty())
  {
      GmicStdLib::loadStdLib();
  }
  FiltersModelReader filterModelReader(filtersModel);
  filterModelReader.parseFiltersDefinitions(GmicStdLib::Array);
}

void GmicHelper::Filter::clear()
{
  name.clear();
  command.clear();
  previewCommand.clear();
  parameters.clear();
  defaultParameterValues.clear();
  hash.clear();
  plainTextName.clear();
  previewFactor = Gmic::UnspecifiedPreviewMode;
  defaultInputMode = Gmic::UnspecifiedInputMode;
  isAFave = false;
}

