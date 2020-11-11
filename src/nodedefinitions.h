/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

namespace Cascade
{
    ////////////////////////////////////
    // Node Input Types
    ////////////////////////////////////
    enum NodeInputType
    {
        NODE_INPUT_TYPE_NONE,
        NODE_INPUT_TYPE_RGB_FRONT,
        NODE_INPUT_TYPE_RGB_BACK,
        NODE_INPUT_TYPE_ALPHA
    };

    ////////////////////////////////////
    // Node Output Types
    ////////////////////////////////////
    enum NodeOutputType
    {
        NODE_OUTPUT_TYPE_RGB
    };

    ////////////////////////////////////
    // Front Input Traits
    ////////////////////////////////////
    enum FrontInputTrait
    {
        FRONT_INPUT_ALWAYS_CLEAR,
        FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR
    };

    ////////////////////////////////////
    // Back Input Traits
    ////////////////////////////////////
    enum BackInputTrait
    {
        BACK_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR
    };

    ////////////////////////////////////
    // Alpha Input Traits
    ////////////////////////////////////
    enum AlphaInputTrait
    {
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_INPUT_RENDER_UPSTREAM_OR_CLEAR
    };

    ////////////////////////////////////
    // Alpha Output Traits
    ////////////////////////////////////
    enum AlphaOutputTrait
    {
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR
    };

    ////////////////////////////////////
    // Output Traits
    ////////////////////////////////////
    enum RGBOutputTrait
    {
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_IF_FRONT_DISCONNECTED
    };

    ////////////////////////////////////
    // Global UI element types
    ////////////////////////////////////
    enum UIElementType
    {
        UI_ELEMENT_TYPE_PROPERTIES_HEADING,
        UI_ELEMENT_TYPE_SLIDERSPIN_INT,
        UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE,
        UI_ELEMENT_TYPE_SPINBOX,
        UI_ELEMENT_TYPE_FILEBOX,
        UI_ELEMENT_TYPE_COLOR_BUTTON,
        UI_ELEMENT_TYPE_WRITE_PROPERTIES,
        UI_ELEMENT_TYPE_COMBOBOX,
        UI_ELEMENT_TYPE_CHANNEL_SELECT,
        UI_ELEMENT_TYPE_SLIDER_BOX,
        UI_ELEMENT_TYPE_COLOR_PROPERTIES
    };

    ////////////////////////////////////
    // Node categories
    ////////////////////////////////////
    enum NodeCategory
    {
        NODE_CATEGORY_IO,
        NODE_CATEGORY_GENERATE,
        NODE_CATEGORY_COLOR,
        NODE_CATEGORY_FILTER,
        NODE_CATEGORY_MERGE,
        NODE_CATEGORY_TRANSFORM,
        NODE_CATEGORY_CHANNEL,
        NODE_CATEGORY_MAX
    };

    ////////////////////////////////////
    // Category names
    ////////////////////////////////////
    const QMap<NodeCategory, QString> categoryStrings =
    {
        { NODE_CATEGORY_IO, "IO" },
        { NODE_CATEGORY_GENERATE, "Generate" },
        { NODE_CATEGORY_COLOR, "Color" },
        { NODE_CATEGORY_FILTER, "Filter" },
        { NODE_CATEGORY_MERGE, "Merge" },
        { NODE_CATEGORY_TRANSFORM, "Transform" },
        { NODE_CATEGORY_CHANNEL, "Channel" }
    };

    ////////////////////////////////////
    // Global node types
    ////////////////////////////////////
    enum NodeType
    {
        NODE_TYPE_CROP,
        NODE_TYPE_READ,
        NODE_TYPE_BLUR,
        NODE_TYPE_COLOR,
        NODE_TYPE_RESIZE,
        NODE_TYPE_ROTATE,
        NODE_TYPE_SHARPEN,
        NODE_TYPE_MERGE,
        NODE_TYPE_SHUFFLE,
        NODE_TYPE_PIXELATE,
        NODE_TYPE_SOLARIZE,
        NODE_TYPE_CONSTANT,
        NODE_TYPE_NOISE,
        NODE_TYPE_DIFFERENCE,
        NODE_TYPE_PREMULT,
        NODE_TYPE_UNPREMULT,
        NODE_TYPE_WRITE,
        NODE_TYPE_INVERT,
        NODE_TYPE_EDGE_DETECT,
        NODE_TYPE_DIRECTIONAL_BLUR,
        NODE_TYPE_CHANNEL_COPY,
        NODE_TYPE_MAX
    };

    ////////////////////////////////////
    // Node names
    ////////////////////////////////////
    const QMap<NodeType, QString> nodeStrings =
    {
        { NODE_TYPE_CROP, "Crop" },
        { NODE_TYPE_READ, "Read" },
        { NODE_TYPE_BLUR, "Blur" },
        { NODE_TYPE_COLOR, "Color" },
        { NODE_TYPE_RESIZE, "Resize" },
        { NODE_TYPE_ROTATE, "Rotate" },
        { NODE_TYPE_SHARPEN, "Sharpen" },
        { NODE_TYPE_MERGE, "Merge" },
        { NODE_TYPE_SHUFFLE, "Shuffle" },
        { NODE_TYPE_PIXELATE, "Pixelate" },
        { NODE_TYPE_SOLARIZE, "Solarize" },
        { NODE_TYPE_CONSTANT, "Constant" },
        { NODE_TYPE_NOISE, "Noise" },
        { NODE_TYPE_DIFFERENCE, "Difference" },
        { NODE_TYPE_PREMULT, "Premult" },
        { NODE_TYPE_UNPREMULT, "Unpremult" },
        { NODE_TYPE_WRITE, "Write" },
        { NODE_TYPE_INVERT, "Invert" },
        { NODE_TYPE_EDGE_DETECT, "Edge Detect" },
        { NODE_TYPE_DIRECTIONAL_BLUR, "Directional Blur" },
        { NODE_TYPE_CHANNEL_COPY, "Channel Copy" }
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
        AlphaOutputTrait alphaOutputTrait;
        RGBOutputTrait rgbOutputTrait;
        QString shaderPath;
        int numShaderPasses;
    };

    ////////////////////////////////////
    // Node-specific structs
    ////////////////////////////////////
    const NodeInitProperties cropNodeInitProperties =
    {
        NODE_TYPE_CROP,
        nodeStrings[NODE_TYPE_CROP],
        NODE_CATEGORY_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_CROP] },
            { UI_ELEMENT_TYPE_SPINBOX, "Left,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Top,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Right,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Bottom,0,100000,1,0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/crop_comp.spv",
        1
    };

    const NodeInitProperties readNodeInitProperties =
    {
        NODE_TYPE_READ,
        nodeStrings[NODE_TYPE_READ],
        NODE_CATEGORY_IO,
        { },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_READ] },
            { UI_ELEMENT_TYPE_FILEBOX, "" },
            { UI_ELEMENT_TYPE_COMBOBOX, "Color Space:,sRGB,Linear,rec709,Cineon,Gamma 1.8,Gamma 2.2,Panalog,REDLog,ViperLog,AlexaV3LogC,PLogLin,SLog,Raw,0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_ALWAYS_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/read_comp.spv",
        1
    };

    const NodeInitProperties blurNodeInitProperties =
    {
        NODE_TYPE_BLUR,
        nodeStrings[NODE_TYPE_BLUR],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_BLUR] },
            { UI_ELEMENT_TYPE_CHANNEL_SELECT, "" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Strength,0,100,1,3" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/blur_comp.spv",
        2
    };

    const NodeInitProperties colorNodeInitProperties =
    {
        NODE_TYPE_COLOR,
        nodeStrings[NODE_TYPE_COLOR],
        NODE_CATEGORY_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK,
          NODE_INPUT_TYPE_ALPHA },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_COLOR] },
            { UI_ELEMENT_TYPE_COLOR_PROPERTIES, "" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/color_comp.spv",
        1
    };

    const NodeInitProperties resizeNodeInitProperties =
    {
        NODE_TYPE_RESIZE,
        nodeStrings[NODE_TYPE_RESIZE],
        NODE_CATEGORY_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_RESIZE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Factor,0.1,2.0,0.01,1.0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/resize_comp.spv",
        1
    };

    const NodeInitProperties rotateNodeInitProperties =
    {
        NODE_TYPE_ROTATE,
        nodeStrings[NODE_TYPE_ROTATE],
        NODE_CATEGORY_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_ROTATE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Angle,0,360,1,0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/rotate_comp.spv",
        1
    };

    const NodeInitProperties sharpenNodeInitProperties =
    {
        NODE_TYPE_SHARPEN,
        nodeStrings[NODE_TYPE_SHARPEN],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SHARPEN] },
            { UI_ELEMENT_TYPE_CHANNEL_SELECT, "" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Amount,0,100,1,1" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/sharpen_comp.spv",
        1
    };

    const NodeInitProperties mergeNodeInitProperties =
    {
        NODE_TYPE_MERGE,
        nodeStrings[NODE_TYPE_MERGE],
        NODE_CATEGORY_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK,
          NODE_INPUT_TYPE_RGB_FRONT },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_MERGE] },
            { UI_ELEMENT_TYPE_COMBOBOX, "Mode:,Over,Add,Divide,Minus,Multiply,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "X Offset,-10000,10000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Y Offset,-10000,10000,1,0" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Opacity,0.0,1.0,0.01,1.0" }
        },
        FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_IF_FRONT_DISCONNECTED,
        ":/shaders/merge_comp.spv",
        1
    };

    const NodeInitProperties shuffleNodeInitProperties =
    {
        NODE_TYPE_SHUFFLE,
        nodeStrings[NODE_TYPE_SHUFFLE],
        NODE_CATEGORY_CHANNEL,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SHUFFLE] },
            { UI_ELEMENT_TYPE_COMBOBOX, "Red:,Red,Green,Blue,Alpha,0" },
            { UI_ELEMENT_TYPE_COMBOBOX, "Green:,Red,Green,Blue,Alpha,1" },
            { UI_ELEMENT_TYPE_COMBOBOX, "Blue:,Red,Green,Blue,Alpha,2" },
            { UI_ELEMENT_TYPE_COMBOBOX, "Alpha:,Red,Green,Blue,Alpha,3" }
        },
        FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/shuffle_comp.spv",
        1
    };

    const NodeInitProperties pixelateNodeInitProperties =
    {
        NODE_TYPE_PIXELATE,
        nodeStrings[NODE_TYPE_PIXELATE],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_PIXELATE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Filter Size,0,100,1,5" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/pixelate_comp.spv",
        1
    };

    const NodeInitProperties solarizeNodeInitProperties =
    {
        NODE_TYPE_SOLARIZE,
        nodeStrings[NODE_TYPE_SOLARIZE],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SOLARIZE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Red Thresh,0.0,1.0,0.01,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Green Thresh,0.0,1.0,0.01,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Blue Thresh,0.0,1.0,0.01,0.5" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/solarize_comp.spv",
        1
    };

    const NodeInitProperties constantNodeInitProperties =
    {
        NODE_TYPE_CONSTANT,
        nodeStrings[NODE_TYPE_CONSTANT],
        NODE_CATEGORY_GENERATE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_CONSTANT] },
            { UI_ELEMENT_TYPE_COLOR_BUTTON, "Color" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/constant_comp.spv",
        1
    };

    const NodeInitProperties noiseNodeInitProperties =
    {
        NODE_TYPE_NOISE,
        nodeStrings[NODE_TYPE_NOISE],
        NODE_CATEGORY_GENERATE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_NOISE] },
            { UI_ELEMENT_TYPE_COMBOBOX, "Type:,Worley-Perlin,Worley,0" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Size,1,256,1,32"}
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noise_comp.spv",
        1
    };

    const NodeInitProperties differenceNodeInitProperties =
    {
        NODE_TYPE_DIFFERENCE,
        nodeStrings[NODE_TYPE_DIFFERENCE],
        NODE_CATEGORY_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK,
          NODE_INPUT_TYPE_RGB_FRONT },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_DIFFERENCE] }
        },
        FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_IF_FRONT_DISCONNECTED,
        ":/shaders/difference_comp.spv",
        1
    };

    const NodeInitProperties premultNodeInitProperties =
    {
        NODE_TYPE_PREMULT,
        nodeStrings[NODE_TYPE_PREMULT],
        NODE_CATEGORY_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_PREMULT] }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/premult_comp.spv",
        1
    };

    const NodeInitProperties unpremultNodeInitProperties =
    {
        NODE_TYPE_UNPREMULT,
        nodeStrings[NODE_TYPE_UNPREMULT],
        NODE_CATEGORY_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_UNPREMULT] }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/unpremult_comp.spv",
        1
    };

    const NodeInitProperties writeNodeInitProperties =
    {
        NODE_TYPE_WRITE,
        nodeStrings[NODE_TYPE_WRITE],
        NODE_CATEGORY_IO,
        { NODE_INPUT_TYPE_RGB_BACK },
        { },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_WRITE] },
            { UI_ELEMENT_TYPE_WRITE_PROPERTIES, "" },
            { UI_ELEMENT_TYPE_COMBOBOX, "Color Space:,sRGB,Linear,rec709,Cineon,Gamma 1.8,Gamma 2.2,Panalog,REDLog,ViperLog,AlexaV3LogC,PLogLin,SLog,Raw,0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noop_comp.spv",
        1
    };

    const NodeInitProperties invertNodeInitProperties =
    {
        NODE_TYPE_INVERT,
        nodeStrings[NODE_TYPE_INVERT],
        NODE_CATEGORY_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_INVERT] },
            { UI_ELEMENT_TYPE_CHANNEL_SELECT, "" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/invert_comp.spv",
        1
    };

    const NodeInitProperties edgeDetectNodeInitProperties =
    {
        NODE_TYPE_EDGE_DETECT,
        nodeStrings[NODE_TYPE_EDGE_DETECT],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_EDGE_DETECT] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Gain,0.0,10.0,0.01,1.0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/sobel_comp.spv",
        1
    };

    const NodeInitProperties directionalBlurNodeInitProperties =
    {
        NODE_TYPE_DIRECTIONAL_BLUR,
        nodeStrings[NODE_TYPE_DIRECTIONAL_BLUR],
        NODE_CATEGORY_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_DIRECTIONAL_BLUR] },
            { UI_ELEMENT_TYPE_CHANNEL_SELECT, "" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Angle,0,360,1,0" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Strength,1,100,1,1" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Iterations,1,100,1,10" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Gain,0.0,2.0,0.01,1.0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_INPUT_ALWAYS_CLEAR,
        ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/directionalblur_comp.spv",
        1
    };

    const NodeInitProperties channelCopyNodeInitProperties =
        {
            NODE_TYPE_CHANNEL_COPY,
            nodeStrings[NODE_TYPE_CHANNEL_COPY],
            NODE_CATEGORY_CHANNEL,
            { NODE_INPUT_TYPE_RGB_BACK,
              NODE_INPUT_TYPE_RGB_FRONT},
            { NODE_OUTPUT_TYPE_RGB },
            {
                { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_CHANNEL_COPY] },
                { UI_ELEMENT_TYPE_COMBOBOX, "Red:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,0" },
                { UI_ELEMENT_TYPE_COMBOBOX, "Green:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,1" },
                { UI_ELEMENT_TYPE_COMBOBOX, "Blue:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,2" },
                { UI_ELEMENT_TYPE_COMBOBOX, "Alpha:,Back Red,Back Green,Back Blue,Back Alpha,Front Red,Front Green,Front Blue,Front Alpha,3" }
            },
            FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR,
            BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
            ALPHA_INPUT_ALWAYS_CLEAR,
            ALPHA_OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
            OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
            ":/shaders/channelcopy_comp.spv",
            1
        };

    static NodeInitProperties getPropertiesForType(const NodeType t)
    {
        if(t == NODE_TYPE_CROP)
        {
            return cropNodeInitProperties;
        }
        else if(t == NODE_TYPE_READ)
        {
            return readNodeInitProperties;
        }
        else if(t == NODE_TYPE_BLUR)
        {
            return blurNodeInitProperties;
        }
        else if(t == NODE_TYPE_COLOR)
        {
            return colorNodeInitProperties;
        }
        else if(t == NODE_TYPE_RESIZE)
        {
            return resizeNodeInitProperties;
        }
        else if(t == NODE_TYPE_ROTATE)
        {
            return rotateNodeInitProperties;
        }
        else if(t == NODE_TYPE_SHARPEN)
        {
            return sharpenNodeInitProperties;
        }
        else if(t == NODE_TYPE_MERGE)
        {
            return mergeNodeInitProperties;
        }
        else if(t == NODE_TYPE_SHUFFLE)
        {
            return shuffleNodeInitProperties;
        }
        else if(t == NODE_TYPE_PIXELATE)
        {
            return pixelateNodeInitProperties;
        }
        else if(t == NODE_TYPE_SOLARIZE)
        {
            return solarizeNodeInitProperties;
        }
        else if(t == NODE_TYPE_CONSTANT)
        {
            return constantNodeInitProperties;
        }
        else if(t == NODE_TYPE_NOISE)
        {
            return noiseNodeInitProperties;
        }
        else if(t == NODE_TYPE_DIFFERENCE)
        {
            return differenceNodeInitProperties;
        }
        else if(t == NODE_TYPE_PREMULT)
        {
            return premultNodeInitProperties;
        }
        else if(t == NODE_TYPE_UNPREMULT)
        {
            return unpremultNodeInitProperties;
        }
        else if(t == NODE_TYPE_WRITE)
        {
            return writeNodeInitProperties;
        }
        else if(t == NODE_TYPE_INVERT)
        {
            return invertNodeInitProperties;
        }
        else if(t == NODE_TYPE_EDGE_DETECT)
        {
            return edgeDetectNodeInitProperties;
        }
        else if(t == NODE_TYPE_DIRECTIONAL_BLUR)
        {
            return directionalBlurNodeInitProperties;
        }
        else if(t == NODE_TYPE_CHANNEL_COPY)
        {
            return channelCopyNodeInitProperties;
        }
        throw std::runtime_error("Node type not found.");
    }
}

#endif // NODEDEFINITIONS_H
