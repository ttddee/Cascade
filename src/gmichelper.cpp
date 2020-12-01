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

    gmicInstance = std::unique_ptr<gmic>(new gmic(
                                             QString("").toLocal8Bit().constData(),
                                             GmicStdLib::Array.constData(),
                                             true,
                                             0, 0, 0.f));

    //filtersModel.printFilterNames();

    filtersModel.excludeFilters(excludedFilters);
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

void GmicHelper::setCurrentFilter(const QString& hash)
{
    if (hash.isEmpty())
    {
        currentFilter.clear();
    }
    else
    {
        if (filtersModel.contains(hash))
        {
            const FiltersModel::Filter & filter = filtersModel.getFilterFromHash(hash);
            currentFilter.command = filter.command();
            currentFilter.defaultParameterValues = ParametersCache::getValues(hash);
            currentFilter.defaultVisibilityStates = ParametersCache::getVisibilityStates(hash);
            currentFilter.defaultInputMode = filter.defaultInputMode();
            currentFilter.hash = hash;
            currentFilter.isAFave = false;
            currentFilter.name = filter.name();
            currentFilter.plainTextName = filter.plainText();
            currentFilter.parameters = filter.parameters();
            currentFilter.previewCommand = filter.previewCommand();
            currentFilter.isAccurateIfZoomed = filter.isAccurateIfZoomed();
            currentFilter.previewFactor = filter.previewFactor();
        } else
        {
            currentFilter.clear();
        }
    }
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

