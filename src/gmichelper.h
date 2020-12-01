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
        "e4094f01a2c1745271f9e2634e9c0043", // Circle Transform
        "45aa3f9918dfe8f8d627657977d1222f", // Conformal Maps
        "f948fd011146b97797594a7e104a60d3", // Distort Lens
        "38505f71b8147000023a8be3081c6d5a", // Euclidean - Polar
        "f4e7898ac1f29884dd4a4e8915511d00", // Drop Water
        "8a15b06f4a9574570bcbbd88f6e85879", // Fish Eye
        "c11fb8b0c1d33c5450ef8c2c65bfee37", // Flower
        "4841fec75fff1eaeded98c46beac0d3d", // Kaleidoscope [Blended]
        "e7334d9acd4a439cfbe5f6efc2ca53bc", // Kaleidoscope [Polar]
        "3dae5cc59d02d659f0594b6f1c989029", // Morph [Interactive]
        "1a548fd7f271eaf2b9b2e98897b3e6de", // Perspective
        "0ba8d8442d4dcdfb501986e74be26f73", // Polar Transform
        "ca2c2d7275bd8ff7c4ee6dd2c155dec9", // Quadrangle
        "399b31ac45307fc0c0c62fa8ce8b3ced", // Spherize
        "75255533267956f8e6ffb5637ee67818", // Stereographic Projection
        "1d8ba6854d9f5740f018012693729699", // Symmetrize
        "7e43e75e7d632ecb8a28947252c667dc", // Twirl
        "14724d115921e52e05929381a93ea773", // Warp [Interactive]
        "51c5f6c361ae8746179f74b6c12e11dc", // Zoom
        "96feecfea9ddc41d984499863993f894", // Blur [Angular]
        "66c6fe427f8154f52793df19cdaee110", // Blur [Depth-Of-Field]
        "c2cb6e622a08f444240ddc907924a3ea", // Blur [Multidirectional]
        "9222d87cb06c1ea4821431bd5fc795a3", // Blur [Radial]
        "038724dc2ef1edd1791945af4a52028c", // Chromatic Aberrations
        "573425d3f3e1b013204f7403b13127ff", // Flip and Rotate Blocs
        "b5d005c5e8ddfe499f42f6c40c6f8aab", // Mess with Bits
        "445289b982ba728e199da3cd3804eb54", // Self Glitching
        "22f3683b10c4c616109e21cba8cbe346", // Streak
        "d20fd6a7bbc5b3806deeba0a139d063f", // Details Equalizer
        "ba4a7d6c6c59e3b761899430aebfdefa", // Freaky Details
        "2fbd3c869750b2d8b92bfdd4fbd9398c", // Sharpen [Deblur]
        "d814978b1b9f3ba80a787e77da1cc690", // Sharpen [Gold-Meinel]
        "a33b9ae38057ba79be5b7a67ad78ad85", // Split Details [Alpha]
        "b141311333f8ec8693822a0977e32cd4", // Droste
        "72bb47d724be401c30851e930e64a5c3", // Frame [Mirror]
        "3675a1eeef71a5362a6c2b088700dac8", // Frame [Painting]
        "8a79a1ce49d96c45d92879e7a225f8f1", // Frame [Pattern]
        "183e942d09a4518f04f11d59390a7cc3", // Frame [Smooth]
        "1ef0753e56647589546909f03cbc2afa", // Polaroid
        "5726ddc485aa1ba63aee5534c4085549", // Tunnel
        "9024a0df0f7478d24445be766b6df58f", // Fourier Watermark
        "937b9fc89ba953db855b5874c3026c96", // Align Layers
        "e5ade55ca731de57957d83bd8a756456", // Blend [Edges]
        "52de2503e2e800862d9b93f5168e6788", // Blend [Average All]
        "acd43799a5f22596dd13ca17b9de0560", // Blend [Fade]
        "a02c90edc18b693ca82447f859ebf0a1", // Blend [Median]
        "a7223bf5c71dd6620c0321db660dad0c", // Colors to Layers
        "1030fcca819880b1edad1bb59d1d8fe8", // Fade Layers
        "cffcd21e93eaf79feba009f02675c11d", // Layers to Tiles
        "2dc05ce11791d256040f0c746ed03ff7", // Morph Layers
        "afd3ed4bb2c3fcc90ed6c6a15c860561", // Multiscale Operator
        "5a080044a2028f9e5d787fcb656ac0e6", // Pack
        "920afbe4651e58ab5e55335507beb97d", // Stroke
        "9d4f3cb0e0a12c3aec365f2f9c5d832a", // Tiles to Layers
        "f92f00d56307af46f7689445b3c31a1f", // Tones to Layers
        "0a85a9e3c21326d2afbd3935c49081c7", // Blend [Seamless]
        "1bea93cd892f74713feebb94a5a070de", // Blend [Standard]
        "6a8f10fedd55da9798b8fc83aceeceb6", // Swiss Mask
        "2474dbc53a751ec0030de4e09f9557b4", // Drop Shadow
        "8eca95077c979703645597162591e89e", // Drop Shadow 3D
        "d0c09771c47ce6ecac065387f07e22f3", // Equalize Shadow
        "10313329fc804d66dd888fa839803c14", // Illuminate 2D Shape
        "02ab338bcfc58b966a9a184ba8bda3a8", // Slice Luminosity
        "dc133482f048f70e4ba700cc926aadac", // Pack Sprites
        "e07a70e6886787f871adaa51a32df17d", // Random Pattern
        "a31748a70edc013489afa21fcd302486", // 3D Blocks
        "75870865fae9e985321172b789ca7f9d", // 3D Colored Object
        "04073efb54d303c4a9b48ceb2b30a61a", // 3D Elevation
        "026816f6ef878b2cd1dd26a58d27da26", // 3D Elevation [Animated]
        "451325bf38dda0749b1ef516879698fa", // 3D Extrusion
        "2cb4f1e1801f1f2e203dd1f22e715827", // 3D Elevation [Animated]
        "e0b34bb4fcc9ba8e6676287d25234b60", // 3D Image Object
        "dd37ac9c3f81fbf9afa2a8025c2b861e", // 3D Image Object [Animated]
        "c4a0edb0ca0be54db497cc10c5ff399a", // 3D Lathing
        "abb68de377f3edbefbfd634f9121c0d7", // 3D Random Object
        "a7684e3856a79fd39c9a500353427927", // Gradient [from Line]
        "ddfeddd305451d27a41a4f017ccf3aaa", // Lightning
        "3d1f9387bcb64507e10a4a5adf312161", // Mandelbrot - Julia Sets
        "c9ff1f7d035c060feeb6e788415780a9", // Neon Lightning
        "f96af0b2ebbe3a231168d017ab9d7e93", // Newton Fractal
        "c21cf047b7d1560c15e1edc4b7556623", // Symmetric 2D Shape
        "e4cd6f3ed4ecf2f177e56112b1c25189", // Tree
        "048728ad5204efe1cf6cbe74e1e4ebc6", // Bayer Reconstruction
        "f0cf70e553a5441922aa33d819f7396a", // Inpaint [Morphological]
        "0cad14ccbf6a69d4eada27fe1fbcc66d", // Inpaint [Multi-Scale]
        "42cd1b3bab3ca1c3367bc20d0e4082c7", // Inpaint [Patch-Based]
        "9474f426216e0029ced92f3fbfb14203", // Inpaint
        "d469e854f983f559f19cdf0e6dc83a7b", // Smooth [Guided]
        "3624dc8461ed7d4c808fc37f970102ea", // Smooth [Mean-Curvature]
        "d36b5e81f4a3d9ef0c48646f9cb6d267", // Smooth [NL-Means]
        "b77769a2257bb8c739a66e0bf9671ed2", // Smooth [Patch-Based]
        "77000894554f51cd5cf1cdbccc2c3c97", // Smooth [Perona-Malik]
        "a932dfb8ce96d39f4e6d89216e6bcb78", // Smooth [Total Variation]
        "f8fd90d957ec2da5bdc39a48f7a67cbb", // Smooth [Wavelets]
        "35a5ed138a8ca14699187fd6159e904d", // Upscale [DCCI2x]
        "cc35cb730533c6ad73e2ca97988ea244", // Upscale [2x]
        "f73cf080bde40188d82991bcdd1cd271", // 3D Text Pointcloud
        "e11159f6ea33ef26a42c4ebcf9aa3cff", // 3D Tiles
        "62f56525be5bf5ac1fb9b27380cb3e5c", // BandW Pencil [Animated]
        "0f25f29c9fe2e8ef2789e29a08bd577e", // Cartoon [Animated]
        "64a93645a507ce7ccce81e12d8e1a227", // Edges [Animated]
        "3d39b0ca48854ba92dad3c6169aca076", // Lissajous [Animated]
        "779e519afa82eaf46edc9aef4d189c2e", // Moirandeacute; Animation
        "b76ce970b96d6745c4443dd1db30d8f9", // Rodilius [Animated]
        "9aeb44512b3b5fc5030c42c23cce84bc", // Soft Glow [Animated]
        "76b5d4264cdd563bf6696699de3da3ff", // Spatial Transition
        "016ad89d338ae6604589abf42c1ce4d1", // Export RGB-565 File
        "78c4e108c191c50bfb370c9d48045f28", // Games and Demos
        "37ec1014f0e7093438fef812a92fea53", // Import Data
        "b0739daec6286bd65a4d3d4da446fcc5", // Import RGB-565 File
        "3b47397b1187060b81c5cba087f52407", // Intarsia
        "93316966b105ec015f49d4c97d020bf3", // Solve Maze
    };
};

#endif // GMICHELPER_H
