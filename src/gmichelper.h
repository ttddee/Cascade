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
    void setCurrentFilter(const QString& hash);

    std::shared_ptr<gmic> gmicInstance;
    FiltersModel filtersModel;
    Filter currentFilter;

    const std::vector<QString> excludedFilters =
    {
        "83061d9f12eb7d5db1ee0dfa88aed076", // About G'MIC
        "108d46913b4c8793c02b3b00fe97a46c", // Contributors
        "63a7de507c779b16dbad6af308ab3091", // Download External Data
        "a384bc6f7712a4753c7e73b3a9b2d5d3", // Filter Design
        "29154bcf8389287d7eab92f631f30f2f", // Friends Hall of Fame
        "b58157f19289f68c3117cf348508dc7f", // Gmicky - Roddy
        "decfae5d2acec5d01f5abaa4b9d2ee22", // Privacy Notice
        "26dd4f642847acf1d652f853eb9b1d08", // Release Notes
        "8f6c570862cfc5bbacd67a1c9b038b55" // Support Us
    };
};

#endif // GMICHELPER_H
