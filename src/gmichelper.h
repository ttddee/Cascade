#ifndef GMICHELPER_H
#define GMICHELPER_H

#include "gmic/FiltersModel.h"
//#include "gmic/GmicDefinitions.h"

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

    GmicHelper();

private:
    void readFilters();
    void setCurrentFilter(const QString& hash);

    FiltersModel filtersModel;
    Filter currentFilter;
};

#endif // GMICHELPER_H
