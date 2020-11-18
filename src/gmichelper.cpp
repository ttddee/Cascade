#include "gmichelper.h"

#include <iostream>

#include "gmic/GmicStdlib.h"
#include "gmic/FiltersModelReader.h"
#include "gmic/ParametersCache.h"

GmicHelper::GmicHelper()
{
    readFilters();

    std::cout << filtersModel.filterCount() << std::endl;
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

  filtersModel.printFilterNames();
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
  previewFactor = Gmic::DefaultPreviewMode;
  defaultInputMode = Gmic::DefaultInputMode;
  isAFave = false;
}
