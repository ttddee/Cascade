#ifndef GMICHELPER_H
#define GMICHELPER_H

#include <memory>

#include "gmic/FiltersModel.h"
#include "gmic.h"

using namespace Cascade;

class GmicHelper
{
public:
    struct Filter {
        QString name;
        QString plainTextName;
        QString command;
        QString previewCommand;
        QString parameters;
        QList<QString> defaultParameterValues;
        QList<int> defaultVisibilityStates;
        Gmic::InputMode defaultInputMode;
        QString hash;
        bool isAccurateIfZoomed;
        float previewFactor;
        bool isAFave;
        void clear();
        void setInvalid();
        bool isInvalid() const;
        bool isNoApplyFilter() const;
        bool isNoPreviewFilter() const;
        const char * previewFactorString() const;
      };

    static GmicHelper& getInstance();

    GmicHelper(GmicHelper const&) = delete;
    void operator=(GmicHelper const&) = delete;

    std::shared_ptr<gmic> getGmicInstance();
    QSet<QString>& getFilterCategories();
    FiltersModel& getFiltersModel();

    void setUp();

private:
    GmicHelper() {}

    void readFilters();

    FiltersModel filtersModel;
    Filter currentFilter;
    std::shared_ptr<gmic> gmicInstance;

};

#endif // GMICHELPER_H
