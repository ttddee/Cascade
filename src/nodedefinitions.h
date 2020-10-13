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
    // Alpha Traits
    ////////////////////////////////////
    enum AlphaOutputTrait
    {
        ALPHA_RENDER_UPSTREAM_OR_CLEAR
    };

    ////////////////////////////////////
    // Output Traits
    ////////////////////////////////////
    enum RGBOutputTrait
    {
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR
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
        UI_ELEMENT_TYPE_COMBOBOX
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
        { NODE_TYPE_INVERT, "Invert" }
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
        AlphaOutputTrait alphaOutputTrait;
        RGBOutputTrait rgbOutputTrait;
        QString shaderPath;
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/crop_comp.spv"
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
            { UI_ELEMENT_TYPE_FILEBOX, "" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_ALWAYS_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noop_comp.spv"
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
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Strength,0,20,1,3" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/blur_comp.spv"
    };

    const NodeInitProperties colorNodeInitProperties =
    {
        NODE_TYPE_COLOR,
        nodeStrings[NODE_TYPE_COLOR],
        NODE_CATEGORY_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_COLOR] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Gamma,0.0,2.0,0.1,1.0" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Gain,0.0,2.0,0.1,1.0" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Lift,-2.0,2.0,0.1,0.0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/color_comp.spv"
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
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Factor,0.1,2.0,0.1,1.0" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/resize_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/rotate_comp.spv"
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
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Amount,0,100,1,1" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/sharpen_comp.spv"
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
            { UI_ELEMENT_TYPE_SPINBOX, "Y Offset,-10000,10000,1,0" }
        },
        FRONT_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/merge_comp.spv"
    };

    const NodeInitProperties shuffleNodeInitProperties =
    {
        NODE_TYPE_SHUFFLE,
        nodeStrings[NODE_TYPE_SHUFFLE],
        NODE_CATEGORY_COLOR,
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/shuffle_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/pixelate_comp.spv"
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
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Red Thresh,0.0,1.0,0.1,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Green Thresh,0.0,1.0,0.1,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Blue Thresh,0.0,1.0,0.1,0.5" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/solarize_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/constant_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noise_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/difference_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/premult_comp.spv"
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
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/unpremult_comp.spv"
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
            { UI_ELEMENT_TYPE_WRITE_PROPERTIES, "" }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties invertNodeInitProperties =
    {
        NODE_TYPE_INVERT,
        nodeStrings[NODE_TYPE_INVERT],
        NODE_CATEGORY_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_INVERT] }
        },
        FRONT_INPUT_ALWAYS_CLEAR,
        BACK_INPUT_RENDER_UPSTREAM_OR_CLEAR,
        ALPHA_RENDER_UPSTREAM_OR_CLEAR,
        OUTPUT_RENDER_UPSTREAM_OR_CLEAR,
        ":/shaders/invert_comp.spv"
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
        throw std::runtime_error("Node type not found.");
    }
}

#endif // NODEDEFINITIONS_H
