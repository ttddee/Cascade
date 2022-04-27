/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef NODEDEFINITIONS_H
#define NODEDEFINITIONS_H

#include <stdexcept>

#include <QMap>

namespace Cascade {

////////////////////////////////////
// Node Input Types
////////////////////////////////////
enum class NodeInputType
{
    eRgbFront,
    eRgbBack,
    eRgbAlpha
};

////////////////////////////////////
// Node Output Types
////////////////////////////////////
enum class NodeOutputType
{
    eRgb
};

////////////////////////////////////
// Front Input Traits
////////////////////////////////////
enum class FrontInputTrait
{
    eAlwaysClear,
    eRenderUpstreamOrClear
};

////////////////////////////////////
// Back Input Traits
////////////////////////////////////
enum class BackInputTrait
{
    eAlwaysClear,
    eRenderUpstreamOrClear
};

////////////////////////////////////
// Alpha Input Traits
////////////////////////////////////
enum class AlphaInputTrait
{
    eAlwaysClear,
    eRenderUpstreamOrClear
};

////////////////////////////////////
// Output Traits
////////////////////////////////////
enum class OutputTrait
{
    eRenderUpstreamOrClear,
    eRenderUpstreamIfFrontDisconnected
};

////////////////////////////////////
// Global UI element types
////////////////////////////////////
enum class UIElementType
{
    ePropertiesHeading,
    eSpinBox,
    eFileBox,
    eColorButton,
    eWriteProperties,
    eComboBox,
    eChannelSelect,
    eSliderBoxDouble,
    eSliderBoxInt,
    eColorProperties,
    eSizeBox,
    eTextBox,
    eCheckBox,
    eTextBrowser, // TODO: This is not used anywhere
    eSeparator,
    eLineEdit, // TODO: This is not used anywhere
    eFolderBox, // TODO: This is not used anywhere
    eResizeProperties,
    eCodeEditor
};

////////////////////////////////////
// Node categories
////////////////////////////////////
enum class NodeCategory
{
    eIO,
    eGenerate,
    eColor,
    eFilter,
    eEffects,
    eMerge,
    eTransform,
    eChannel,
    eIsf,
    eLast
};

////////////////////////////////////
// Category names
////////////////////////////////////
const static QMap<NodeCategory, QString> categoryStrings =
{
    { NodeCategory::eIO,            "IO" },
    { NodeCategory::eGenerate,      "Generate" },
    { NodeCategory::eColor,         "Color" },
    { NodeCategory::eFilter,        "Filter" },
    { NodeCategory::eEffects,       "Effects" },
    { NodeCategory::eMerge,         "Merge" },
    { NodeCategory::eTransform,     "Transform" },
    { NodeCategory::eChannel,       "Channel" },
    { NodeCategory::eIsf,           "ISF" }
};

////////////////////////////////////
// Global node types
////////////////////////////////////
enum class NodeType
{
    eBloom,
    eBlur,
    eChannelCopy,
    eCheckerBoard,
    eChromaKey,
    eClamp,
    eColorBalance,
    eColorCorrect,
    eColorMap,
    eConstant,
    eContours,
    eCrop,
    eDifference,
    eDirectionalBlur,
    eEdgeDetect,
    eErode,
    eExposure,
    eExtractColor,
    eFlip,
    eHueSaturation,
    eInvert,
    eLevels,
    eMerge,
    eNoise,
    eOldFilm,
    ePixelate,
    ePremult,
    eRead,
    eResize,
    eRiverStyx,
    eRotate,
    eShader,
    eSharpen,
    eShuffle,
    eSmartDenoise,
    eSolarize,
    eUnPremult,
    eWrite,
    eIsf,
    eLast
};

////////////////////////////////////
// Node names
////////////////////////////////////
const static QMap<NodeType, QString> nodeStrings =
{
    { NodeType::eBloom,             "Bloom" },
    { NodeType::eBlur,              "Blur" },
    { NodeType::eChannelCopy,       "Channel Copy" },
    { NodeType::eCheckerBoard,      "Checkerboard" },
    { NodeType::eChromaKey,         "Chroma Key" },
    { NodeType::eClamp,             "Clamp" },
    { NodeType::eColorBalance,      "Color Balance" },
    { NodeType::eColorCorrect,      "Color Correct" },
    { NodeType::eColorMap,          "Color Map" },
    { NodeType::eConstant,          "Constant" },
    { NodeType::eContours,          "Contours" },
    { NodeType::eCrop,              "Crop" },
    { NodeType::eDifference,        "Difference Key" },
    { NodeType::eDirectionalBlur,   "Directional Blur" },
    { NodeType::eEdgeDetect,        "Edge Detect" },
    { NodeType::eErode,             "Erode" },
    { NodeType::eExposure,             "Exposure" },
    { NodeType::eExtractColor,      "Extract Color" },
    { NodeType::eFlip,              "Flip" },
    { NodeType::eHueSaturation,     "Hue Saturation" },
    { NodeType::eInvert,            "Invert" },
    { NodeType::eIsf,               "ISF" },
    { NodeType::eLevels,            "Levels" },
    { NodeType::eMerge,             "Merge" },
    { NodeType::eNoise,             "Noise" },
    { NodeType::eOldFilm,           "Old Film" },
    { NodeType::ePixelate,          "Pixelate" },
    { NodeType::ePremult,           "Premult" },
    { NodeType::eRead,              "Read Image" },
    { NodeType::eResize,            "Resize" },
    { NodeType::eRiverStyx,         "River Styx" },
    { NodeType::eRotate,            "Rotate" },
    { NodeType::eShader,            "GLSL Shader" },
    { NodeType::eSharpen,           "Sharpen" },
    { NodeType::eShuffle,           "Shuffle" },
    { NodeType::eSmartDenoise,      "Smart Denoise" },
    { NodeType::eSolarize,          "Solarize" },
    { NodeType::eUnPremult,         "Unpremult" },
    { NodeType::eWrite,             "Write Image" }
};

////////////////////////////////////
// Struct to hold initialization values
////////////////////////////////////
struct NodeInitProperties
{
    NodeType nodeType;
    QString title;
    NodeCategory category;
    std::vector<NodeInputType> nodeInputs;
    std::vector<NodeOutputType> nodeOutputs;
    std::vector<std::pair<UIElementType, QString>> uiElements;
    FrontInputTrait frontInputTrait;
    BackInputTrait backInputTrait;
    AlphaInputTrait alphaInputTrait;
    OutputTrait rgbOutputTrait;
    QString shaderPath;
    int numShaderPasses;
};

////////////////////////////////////
// Node-specific structs
////////////////////////////////////
const static NodeInitProperties cropNodeInitProperties =
{
    NodeType::eCrop,
    nodeStrings[NodeType::eCrop],
    NodeCategory::eTransform,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eCrop] },
        { UIElementType::eSpinBox, "Left,0,100000,1,0" },
        { UIElementType::eSpinBox, "Top,0,100000,1,0" },
        { UIElementType::eSpinBox, "Right,0,100000,1,0" },
        { UIElementType::eSpinBox, "Bottom,0,100000,1,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/crop_comp.spv",
    1
};

const static NodeInitProperties readNodeInitProperties =
{
    NodeType::eRead,
    nodeStrings[NodeType::eRead],
    NodeCategory::eIO,
    { },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eRead] },
        { UIElementType::eFileBox, "" },
        { UIElementType::eComboBox, "Color Space:,sRGB,Linear,rec709,Gamma 1.8,Gamma 2.2,Panalog,REDLog,ViperLog,AlexaV3LogC,PLogLin,SLog,Raw,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eAlwaysClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/read_comp.spv",
    1
};

const static NodeInitProperties blurNodeInitProperties =
{
    NodeType::eBlur,
    nodeStrings[NodeType::eBlur],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eBlur] },
        { UIElementType::eChannelSelect, "0" },
        { UIElementType::eSliderBoxInt, "Strength,0,100,1,3" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/blur_comp.spv",
    1
};

const static NodeInitProperties colorNodeInitProperties =
{
    NodeType::eColorCorrect,
    nodeStrings[NodeType::eColorCorrect],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbAlpha },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eColorCorrect] },
        { UIElementType::eChannelSelect, "1" },
        { UIElementType::eColorProperties, "" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/color_comp.spv",
    1
};

const static NodeInitProperties resizeNodeInitProperties =
{
    NodeType::eResize,
    nodeStrings[NodeType::eResize],
    NodeCategory::eTransform,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eResize] },
        { UIElementType::eResizeProperties, "" },
        { UIElementType::eComboBox, "Filter,Bicubic,Lanczos,Bilinear,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/resize_comp.spv",
    1
};

const static NodeInitProperties rotateNodeInitProperties =
{
    NodeType::eRotate,
    nodeStrings[NodeType::eRotate],
    NodeCategory::eTransform,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eRotate] },
        { UIElementType::eSliderBoxInt, "Angle,0,360,1,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/rotate_comp.spv",
    1
};

const static NodeInitProperties sharpenNodeInitProperties =
{
    NodeType::eSharpen,
    nodeStrings[NodeType::eSharpen],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eSharpen] },
        { UIElementType::eChannelSelect, "0" },
        { UIElementType::eSliderBoxInt, "Amount,0,100,1,1" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/sharpen_comp.spv",
    1
};

const static NodeInitProperties mergeNodeInitProperties =
{
    NodeType::eMerge,
    nodeStrings[NodeType::eMerge],
    NodeCategory::eMerge,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbFront },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eMerge] },
        { UIElementType::eComboBox, "Mode:,Over,Add,Divide,Minus,Multiply,0" },
        { UIElementType::eSpinBox, "X Offset,-10000,10000,1,0" },
        { UIElementType::eSpinBox, "Y Offset,-10000,10000,1,0" },
        { UIElementType::eSliderBoxDouble, "Opacity,0.0,1.0,0.01,1.0" }
    },
    FrontInputTrait::eRenderUpstreamOrClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamIfFrontDisconnected,
    ":/shaders/merge_comp.spv",
    1
};

const static NodeInitProperties shuffleNodeInitProperties =
{
    NodeType::eShuffle,
    nodeStrings[NodeType::eShuffle],
    NodeCategory::eChannel,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eShuffle] },
        { UIElementType::eComboBox, "Red:,Red,Green,Blue,Alpha,0" },
        { UIElementType::eComboBox, "Green:,Red,Green,Blue,Alpha,1" },
        { UIElementType::eComboBox, "Blue:,Red,Green,Blue,Alpha,2" },
        { UIElementType::eComboBox, "Alpha:,Red,Green,Blue,Alpha,3" }
    },
    FrontInputTrait::eRenderUpstreamOrClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/shuffle_comp.spv",
    1
};

const static NodeInitProperties pixelateNodeInitProperties =
{
    NodeType::ePixelate,
    nodeStrings[NodeType::ePixelate],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::ePixelate] },
        { UIElementType::eSliderBoxInt, "Filter Size,0,100,1,5" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/pixelate_comp.spv",
    1
};

const static NodeInitProperties solarizeNodeInitProperties =
{
    NodeType::eSolarize,
    nodeStrings[NodeType::eSolarize],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eSolarize] },
        { UIElementType::eSliderBoxDouble, "Red Thresh,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Green Thresh,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Blue Thresh,0.0,1.0,0.01,0.5" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/solarize_comp.spv",
    1
};

const static NodeInitProperties constantNodeInitProperties =
{
    NodeType::eConstant,
    nodeStrings[NodeType::eConstant],
    NodeCategory::eGenerate,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eConstant] },
        { UIElementType::eColorButton, "Color:, 1.0, 1.0, 1.0, 1.0" },
        { UIElementType::eSizeBox, "" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/constant_comp.spv",
    1
};

const static NodeInitProperties noiseNodeInitProperties =
{
    NodeType::eNoise,
    nodeStrings[NodeType::eNoise],
    NodeCategory::eGenerate,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eNoise] },
        { UIElementType::eComboBox, "Type:,Worley-Perlin,Worley,0" },
        { UIElementType::eSliderBoxInt, "Size,1,256,1,32"},
        { UIElementType::eSizeBox, "" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/noise_comp.spv",
    1
};

const static NodeInitProperties differenceNodeInitProperties =
{
    NodeType::eDifference,
    nodeStrings[NodeType::eDifference],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbFront },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eDifference] },
        { UIElementType::eChannelSelect, "1" },
        { UIElementType::eSliderBoxDouble, "Gain,0.0,4.0,0.01,1.0" }
    },
    FrontInputTrait::eRenderUpstreamOrClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamIfFrontDisconnected,
    ":/shaders/difference_comp.spv",
    1
};

const static NodeInitProperties premultNodeInitProperties =
{
    NodeType::ePremult,
    nodeStrings[NodeType::ePremult],
    NodeCategory::eMerge,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::ePremult] }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/premult_comp.spv",
    1
};

const static NodeInitProperties unpremultNodeInitProperties =
{
    NodeType::eUnPremult,
    nodeStrings[NodeType::eUnPremult],
    NodeCategory::eMerge,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eUnPremult] }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/unpremult_comp.spv",
    1
};

const static NodeInitProperties writeNodeInitProperties =
{
    NodeType::eWrite,
    nodeStrings[NodeType::eWrite],
    NodeCategory::eIO,
    { NodeInputType::eRgbBack },
    { },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eWrite] },
        { UIElementType::eWriteProperties, "" },
        { UIElementType::eComboBox, "Color Space:,sRGB,Linear,rec709,Gamma 1.8,Gamma 2.2,Panalog,REDLog,ViperLog,AlexaV3LogC,PLogLin,SLog,Raw,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/noop_comp.spv",
    1
};

const static NodeInitProperties invertNodeInitProperties =
{
    NodeType::eInvert,
    nodeStrings[NodeType::eInvert],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbAlpha },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eInvert] },
        { UIElementType::eChannelSelect, "0" },
        { UIElementType::eCheckBox, "Use Mask,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/invert_comp.spv",
    1
};

const static NodeInitProperties edgeDetectNodeInitProperties =
{
    NodeType::eEdgeDetect,
    nodeStrings[NodeType::eEdgeDetect],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eEdgeDetect] },
        { UIElementType::eSliderBoxDouble, "Intensity X,0.0,10.0,0.01,1.0" },
        { UIElementType::eSliderBoxDouble, "Intensity Y,0.0,10.0,0.01,1.0" },
        { UIElementType::eSliderBoxDouble, "Gain,0.0,10.0,0.01,1.0" },
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/sobel_comp.spv",
    1
};

const static NodeInitProperties directionalBlurNodeInitProperties =
{
    NodeType::eDirectionalBlur,
    nodeStrings[NodeType::eDirectionalBlur],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eDirectionalBlur] },
        { UIElementType::eChannelSelect, "0" },
        { UIElementType::eSliderBoxInt, "Angle,0,360,1,0" },
        { UIElementType::eSliderBoxInt, "Strength,1,100,1,1" },
        { UIElementType::eSliderBoxInt, "Iterations,1,100,1,10" },
        { UIElementType::eSliderBoxDouble, "Gain,0.0,2.0,0.01,1.0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/directionalblur_comp.spv",
    1
};

const static NodeInitProperties channelCopyNodeInitProperties =
    {
        NodeType::eChannelCopy,
        nodeStrings[NodeType::eChannelCopy],
        NodeCategory::eChannel,
        { NodeInputType::eRgbBack,
          NodeInputType::eRgbFront },
        { NodeOutputType::eRgb },
        {
            { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eChannelCopy] },
            { UIElementType::eComboBox, "Red:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,0" },
            { UIElementType::eComboBox, "Green:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,1" },
            { UIElementType::eComboBox, "Blue:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,2" },
            { UIElementType::eComboBox, "Alpha:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,3" }
        },
        FrontInputTrait::eRenderUpstreamOrClear,
        BackInputTrait::eRenderUpstreamOrClear,
        AlphaInputTrait::eAlwaysClear,
        OutputTrait::eRenderUpstreamOrClear,
        ":/shaders/channelcopy_comp.spv",
        1
    };

const static NodeInitProperties riverStyxNodeInitProperties =
{
    NodeType::eRiverStyx,
    nodeStrings[NodeType::eRiverStyx],
    NodeCategory::eGenerate,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eRiverStyx] },
        { UIElementType::eSliderBoxInt, "Seed,1,1000,1,100"},
        { UIElementType::eSizeBox, "" },
        { UIElementType::eTextBox, "Original shader created by:\nhttps://github.com/gurumatcha" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/riverstyx_comp.spv",
    1
};

const static NodeInitProperties clampNodeInitProperties =
{
    NodeType::eClamp,
    nodeStrings[NodeType::eClamp],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eClamp] },
        { UIElementType::eCheckBox, "Black,1," },
        { UIElementType::eCheckBox, "White,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/clamp_comp.spv",
    1
};

const static NodeInitProperties erodeNodeInitProperties =
{
    NodeType::eErode,
    nodeStrings[NodeType::eErode],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eErode] },
        { UIElementType::eComboBox, "Mode:,Erode,Dilate,0" },
        { UIElementType::eSliderBoxDouble, "Amount,1.0,50.0,0.01,5.0" },
        { UIElementType::eComboBox, "Shape:,Disc,Star,Diamond,Hexagon,Square,0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/erodedilate_comp.spv",
    2
};

const static NodeInitProperties chromaKeyNodeInitProperties =
{
    NodeType::eChromaKey,
    nodeStrings[NodeType::eChromaKey],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eChromaKey] },
        { UIElementType::eColorButton, "Key Color, 0.0, 0.0, 0.0, 0.0" },
        { UIElementType::eSliderBoxDouble, "Range A,0.0,1.0,0.01,0.05" },
        { UIElementType::eSliderBoxDouble, "Range B,0.0,1.0,0.01,0.25" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/chromakey_comp.spv",
    1
};

const static NodeInitProperties shaderNodeInitProperties =
{
    NodeType::eShader,
    nodeStrings[NodeType::eShader],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbFront },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eShader] },
        { UIElementType::eCodeEditor, "" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/noop_comp.spv",
    1
};

const static NodeInitProperties flipNodeInitProperties =
{
    NodeType::eFlip,
    nodeStrings[NodeType::eFlip],
    NodeCategory::eTransform,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eFlip] },
        { UIElementType::eCheckBox, "Flip,1," },
        { UIElementType::eCheckBox, "Flop,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/flip_comp.spv",
    1
};

const static NodeInitProperties checkerboardNodeInitProperties =
{
    NodeType::eCheckerBoard,
    nodeStrings[NodeType::eCheckerBoard],
    NodeCategory::eGenerate,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eCheckerBoard] },
        { UIElementType::eSliderBoxDouble, "Size,1.0,1000.0,0.01,20.0" },
        { UIElementType::eSliderBoxDouble, "Aspect,-1.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Phase X,-1.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Phase Y,-1.0,1.0,0.01,0.0" },
        { UIElementType::eColorButton, "Color A:, 0.0, 0.0, 0.0, 0.0" },
        { UIElementType::eColorButton, "Color B:, 1.0, 1.0, 1.0, 1.0" },
        { UIElementType::eSizeBox, "" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/checkerboard_comp.spv",
    1
};

const static NodeInitProperties colorBalanceNodeInitProperties =
{
    NodeType::eColorBalance,
    nodeStrings[NodeType::eColorBalance],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbAlpha },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eColorBalance] },
        { UIElementType::eSliderBoxDouble, "Red,-1.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Green,-1.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Blue,-1.0,1.0,0.01,0.0" },
        { UIElementType::eCheckBox, "Preserve Luminance,0," },
        { UIElementType::eCheckBox, "Use Mask,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/colorbalance_comp.spv",
    1
};

const static NodeInitProperties hueSaturationNodeInitProperties =
{
    NodeType::eHueSaturation,
    nodeStrings[NodeType::eHueSaturation],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbAlpha },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eHueSaturation] },
        { UIElementType::eSliderBoxDouble, "Hue,0.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Saturation,0.0,5.0,0.01,1.0" },
        { UIElementType::eCheckBox, "Invert Luminance,0," },
        { UIElementType::eCheckBox, "Invert Chroma,0," },
        { UIElementType::eCheckBox, "Use Mask,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/huesat_comp.spv",
    1
};

const static NodeInitProperties levelsNodeInitProperties =
{
    NodeType::eLevels,
    nodeStrings[NodeType::eLevels],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack,
      NodeInputType::eRgbAlpha },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eLevels] },
        { UIElementType::eSliderBoxDouble, "In Black,0.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "In White,0.0,1.0,0.01,1.0" },
        { UIElementType::eSliderBoxDouble, "Gamma,0.2,5.0,0.01,1.0" },
        { UIElementType::eSliderBoxDouble, "Out Black,0.0,1.0,0.01,0.0" },
        { UIElementType::eSliderBoxDouble, "Out White,0.0,1.0,0.01,1.0" },
        { UIElementType::eCheckBox, "Luminance Only,0," },
        { UIElementType::eCheckBox, "Use Mask,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eRenderUpstreamOrClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/levels_comp.spv",
    1
};

const static NodeInitProperties colorMapNodeInitProperties =
{
    NodeType::eColorMap,
    nodeStrings[NodeType::eColorMap],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eColorMap] },
        { UIElementType::eColorButton, "Color A:, 0.0, 0.0, 0.0, 0.0" },
        { UIElementType::eColorButton, "Color B:, 1.0, 1.0, 1.0, 1.0" },
        { UIElementType::eSliderBoxDouble, "Gamma,0.2,3.0,0.01,1.0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/colormap_comp.spv",
    1
};

const static NodeInitProperties extractColorNodeInitProperties =
{
    NodeType::eExtractColor,
    nodeStrings[NodeType::eExtractColor],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eExtractColor] },
        { UIElementType::eColorButton, "Color:, 0.0, 0.0, 0.0, 0.0" },
        { UIElementType::eSliderBoxDouble, "Tolerance,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Smoothness,0.0,1.0,0.01,0.0" },
        { UIElementType::eCheckBox, "Hue Only,0," },
        { UIElementType::eCheckBox, "Invert,0," },
        { UIElementType::eCheckBox, "Desaturate,0," }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/extractcolor_comp.spv",
    1
};

const static NodeInitProperties contoursNodeInitProperties =
{
    NodeType::eContours,
    nodeStrings[NodeType::eContours],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eContours] },
        { UIElementType::eSliderBoxDouble, "Threshold,0.0,4.0,0.01,1.0" },
        { UIElementType::eSliderBoxDouble, "Range,0.01,4.0,0.01,1.0" },
        { UIElementType::eColorButton, "Color:, 0.0, 0.0, 0.0, 0.0" },
        { UIElementType::eCheckBox, "Contours Only,0," },
        { UIElementType::eColorButton, "Background:, 1.0, 1.0, 1.0, 1.0" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/contours_comp.spv",
    1
};

const static NodeInitProperties smartDenoiseNodeInitProperties =
{
    NodeType::eSmartDenoise,
    nodeStrings[NodeType::eSmartDenoise],
    NodeCategory::eFilter,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eSmartDenoise] },
        { UIElementType::eSliderBoxDouble, "Sigma,1.0,20.0,0.01,7.0" },
        { UIElementType::eSliderBoxDouble, "Threshold,0.01,1.0,0.01,0.195" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/smartdenoise_comp.spv",
    1
};

const static NodeInitProperties isfNodeInitProperties =
{
    NodeType::eIsf,
    nodeStrings[NodeType::eIsf],
    NodeCategory::eIsf,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eIsf] }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/noop_comp.spv",
    1
};

const static NodeInitProperties bloomNodeInitProperties =
{
    NodeType::eBloom,
    nodeStrings[NodeType::eBloom],
    NodeCategory::eEffects,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eBloom] },
        { UIElementType::eSliderBoxDouble, "Blur,0.0,10.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Intensity,0.01,10.0,0.01,0.5" }
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/bloom_comp.spv",
    1
};

const static NodeInitProperties oldFilmNodeInitProperties =
{
    NodeType::eOldFilm,
    nodeStrings[NodeType::eOldFilm],
    NodeCategory::eEffects,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eOldFilm] },
        { UIElementType::eSliderBoxDouble, "Lines Seed,0.01,1.0,0.01,0.7" },
        { UIElementType::eSliderBoxDouble, "Lines Intensity,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Blotches Seed,0.01,1.0,0.01,0.7" },
        { UIElementType::eSliderBoxDouble, "Blotches Intensity,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Vignette Intensity,0.0,1.0,0.01,0.5" },
        { UIElementType::eSliderBoxDouble, "Grain Intensity,0.0,1.0,0.01,0.25" },
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/oldfilm_comp.spv",
    1
};

const static NodeInitProperties exposureNodeInitProperties =
{
    NodeType::eExposure,
    nodeStrings[NodeType::eExposure],
    NodeCategory::eColor,
    { NodeInputType::eRgbBack },
    { NodeOutputType::eRgb },
    {
        { UIElementType::ePropertiesHeading, nodeStrings[NodeType::eExposure] },
        { UIElementType::eSliderBoxDouble, "Exposure,-10.0,10.0,0.01,0.0" },
    },
    FrontInputTrait::eAlwaysClear,
    BackInputTrait::eRenderUpstreamOrClear,
    AlphaInputTrait::eAlwaysClear,
    OutputTrait::eRenderUpstreamOrClear,
    ":/shaders/exposure_comp.spv",
    1
};

// TODO: Use std::map instead of QMap, then we don't need the lookup function
[[maybe_unused]] static NodeInitProperties getPropertiesForType(const NodeType t)
{
    if(t == NodeType::eCrop)
    {
        return cropNodeInitProperties;
    }
    else if(t == NodeType::eRead)
    {
        return readNodeInitProperties;
    }
    else if(t == NodeType::eBlur)
    {
        return blurNodeInitProperties;
    }
    else if(t == NodeType::eColorCorrect)
    {
        return colorNodeInitProperties;
    }
    else if(t == NodeType::eResize)
    {
        return resizeNodeInitProperties;
    }
    else if(t == NodeType::eRotate)
    {
        return rotateNodeInitProperties;
    }
    else if(t == NodeType::eSharpen)
    {
        return sharpenNodeInitProperties;
    }
    else if(t == NodeType::eMerge)
    {
        return mergeNodeInitProperties;
    }
    else if(t == NodeType::eShuffle)
    {
        return shuffleNodeInitProperties;
    }
    else if(t == NodeType::ePixelate)
    {
        return pixelateNodeInitProperties;
    }
    else if(t == NodeType::eSolarize)
    {
        return solarizeNodeInitProperties;
    }
    else if(t == NodeType::eConstant)
    {
        return constantNodeInitProperties;
    }
    else if(t == NodeType::eNoise)
    {
        return noiseNodeInitProperties;
    }
    else if(t == NodeType::eDifference)
    {
        return differenceNodeInitProperties;
    }
    else if(t == NodeType::ePremult)
    {
        return premultNodeInitProperties;
    }
    else if(t == NodeType::eUnPremult)
    {
        return unpremultNodeInitProperties;
    }
    else if(t == NodeType::eWrite)
    {
        return writeNodeInitProperties;
    }
    else if(t == NodeType::eInvert)
    {
        return invertNodeInitProperties;
    }
    else if(t == NodeType::eEdgeDetect)
    {
        return edgeDetectNodeInitProperties;
    }
    else if(t == NodeType::eDirectionalBlur)
    {
        return directionalBlurNodeInitProperties;
    }
    else if(t == NodeType::eChannelCopy)
    {
        return channelCopyNodeInitProperties;
    }
    else if(t == NodeType::eRiverStyx)
    {
        return riverStyxNodeInitProperties;
    }
    else if(t == NodeType::eClamp)
    {
        return clampNodeInitProperties;
    }
    else if(t == NodeType::eErode)
    {
        return erodeNodeInitProperties;
    }
    else if(t == NodeType::eChromaKey)
    {
        return chromaKeyNodeInitProperties;
    }
    else if(t == NodeType::eShader)
    {
        return shaderNodeInitProperties;
    }
    else if(t == NodeType::eFlip)
    {
        return flipNodeInitProperties;
    }
    else if(t == NodeType::eCheckerBoard)
    {
        return checkerboardNodeInitProperties;
    }
    else if(t == NodeType::eColorBalance)
    {
        return colorBalanceNodeInitProperties;
    }
    else if(t == NodeType::eHueSaturation)
    {
        return hueSaturationNodeInitProperties;
    }
    else if(t == NodeType::eLevels)
    {
        return levelsNodeInitProperties;
    }
    else if(t == NodeType::eColorMap)
    {
        return colorMapNodeInitProperties;
    }
    else if(t == NodeType::eExtractColor)
    {
        return extractColorNodeInitProperties;
    }
    else if(t == NodeType::eContours)
    {
        return contoursNodeInitProperties;
    }
    else if(t == NodeType::eSmartDenoise)
    {
        return smartDenoiseNodeInitProperties;
    }
    else if(t == NodeType::eIsf)
    {
        return isfNodeInitProperties;
    }
    else if(t == NodeType::eBloom)
    {
        return bloomNodeInitProperties;
    }
    else if(t == NodeType::eOldFilm)
    {
        return oldFilmNodeInitProperties;
    }
    else if(t == NodeType::eExposure)
    {
        return exposureNodeInitProperties;
    }
    throw std::runtime_error("Node type not found.");
}
} // namespace Cascade

#endif // NODEDEFINITIONS_H
