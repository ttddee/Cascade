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
        "8f6c570862cfc5bbacd67a1c9b038b55", // Support Us
        //"1bc6366baeb097af2ec755b132bb475e", // Array Random
        "5ea3745daab87ca4abdea9b57767c314", // Drawn Montage
        "bc5271cb409367ee60402caf53db14b6", // Extract Objects
        "25bac3cc35495352939afbe881a31835", // Montage
        "148dc1daf1567563009b2c8f88e2980a", // Tileable Rotation
        "57e5adb465af1d8bba225748ec255981", // Doodle
        "756a15679ca5942af282d0125d777f02", // Painting
        "eb4c69d62d0c62675904223863b32bd6", // Stylize
        "f3e3b4574e56d0e5dcdddd5795ca9f5f", // Warhol
        "070dad59d3b42dd38f23a30b1f5f409c", // BandW Stencil Animated
        "e3b9fcf55aceeb511a9f8ebb75bfdee2", // BandW Stencil
        "bfc28fa8456ce8a4933f056ad68b9860", // Black and White
        "af3ea577ffe0f0acf3dc0653a8455b38", // Colorize Lineart [Auto-Fill]
        "52e8c899daa2ab19312681c59690467e", // Colorize Lineart [Smart Coloring]
        "a18aa71261a73368431e08b686aa8f37", // Colorize Lineart [Propagation]
        "c1cf5897e8a4d82c0cbdde826904c310", // Colorize [Interactive]
        "c702f750c4448f43a31358412438d162", // Colorize [Photographs]
        "937f2b5659190e4b60ca53cb9db2c1cf", // Dithering
        "d49fbf92a4f2787f67ea5b40912dcc31", // Apply External CLUT
        "ad35d41b1e354d4eadc625b707d08c6a", // Boost Chromaticity
        "d22d29c53564329bec739162ec293417", // Boost-Fade
        "5558a5742ac6c864490ca6ef85cf305f", // CLUT from After - Before Layers
        "24d4e46920cf24aa9afe3546225c29f6", // Channels to Layers
        "e2d5b80b9b2072788f494a47a69832cb", // Color Mask
        "fe34284c553e123b1b97658f6acf6730", // Color Presets
        "0a4362de312705cd0757f2d4e2ffd587", // Colorful Blobs
        "c2b7b356506c2e990a574b3dd90ed790", // Curves
        "7bba8370cfc1c85f4ce895bd447cf9c6", // Customize CLUT
        "bf514c8d963c966f5e69c2022cecbd2a", // Decompose Channels
        "9965c3c8bbd8714c93f6434944b333d7", // Mixer [Lab]
        "51a02ee8b185304edbed22fcd50af843", // Mixer [PCA]
        "d252116defd77ea4919e36c1ddc91590", // Select-Replace Color
        "55aaa45f5fc2a45fe1a1aa94b15fe59a", // Simulate Film
        "2c0d436e9ed00dc84231e05a36d91055", // Transfer Colors [Histogram]
        "332a96b159311868ab70b050af0c1648", // Transfer Colors [Variational]
        "9ce51752610ecff3e94f4828f51eb965", // Transfer Colors [PCA]
        "2e72b41322409a1762e34fae57e0774e", // Difference of Gaussians
        "8508bcaa86ca7ca0f616d4fa290a11f7", // Distance Transform
        "21cfdbdae191ae8de5c709f7e6b8376c", // Edges
        "75e587d1f3c779b54730b15ff093069c", // Extract Foreground [Interactive]
    };
};

#endif // GMICHELPER_H
