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
    // Global UI element types
    ////////////////////////////////////
    enum UIElementType
    {
        UI_ELEMENT_TYPE_PROPERTIES_HEADING,
        UI_ELEMENT_TYPE_SLIDERSPIN_INT,
        UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE,
        UI_ELEMENT_TYPE_SPINBOX,
        UI_ELEMENT_TYPE_FILEBOX
    };

    ////////////////////////////////////
    // Node categories
    ////////////////////////////////////
    enum NodeCategory
    {
        NODE_CAT_IO,
        NODE_CAT_GENERATE,
        NODE_CAT_COLOR,
        NODE_CAT_FILTER,
        NODE_CAT_MERGE,
        NODE_CAT_TRANSFORM,
        NODE_CAT_MAX
    };

    ////////////////////////////////////
    // Category names
    ////////////////////////////////////
    const QMap<NodeCategory, QString> categoryStrings =
    {
        { NODE_CAT_IO, "IO" },
        { NODE_CAT_GENERATE, "Generate" },
        { NODE_CAT_COLOR, "Color" },
        { NODE_CAT_FILTER, "Filter" },
        { NODE_CAT_MERGE, "Merge" },
        { NODE_CAT_TRANSFORM, "Transform" },
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
        { NODE_TYPE_WRITE, "Write" }
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
        QString shaderPath;
    };

    ////////////////////////////////////
    // Node-specific structs
    ////////////////////////////////////
    const NodeInitProperties cropNodeInitProperties =
    {
        NODE_TYPE_CROP,
        nodeStrings[NODE_TYPE_CROP],
        NODE_CAT_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_CROP] },
            { UI_ELEMENT_TYPE_SPINBOX, "Left,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Top,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Right,0,100000,1,0" },
            { UI_ELEMENT_TYPE_SPINBOX, "Bottom,0,100000,1,0" }
        },
        ":/shaders/crop_comp.spv"
    };

    const NodeInitProperties readNodeInitProperties =
    {
        NODE_TYPE_READ,
        nodeStrings[NODE_TYPE_READ],
        NODE_CAT_IO,
        { },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_READ] },
            { UI_ELEMENT_TYPE_FILEBOX, "" }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties blurNodeInitProperties =
    {
        NODE_TYPE_BLUR,
        nodeStrings[NODE_TYPE_BLUR],
        NODE_CAT_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_BLUR] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Strength,0,100,1,3" }
        },
        ":/shaders/blur_test_comp.spv"
    };

    const NodeInitProperties colorNodeInitProperties =
    {
        NODE_TYPE_COLOR,
        nodeStrings[NODE_TYPE_COLOR],
        NODE_CAT_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_COLOR] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties resizeNodeInitProperties =
    {
        NODE_TYPE_RESIZE,
        nodeStrings[NODE_TYPE_RESIZE],
        NODE_CAT_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_RESIZE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties rotateNodeInitProperties =
    {
        NODE_TYPE_ROTATE,
        nodeStrings[NODE_TYPE_ROTATE],
        NODE_CAT_TRANSFORM,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_ROTATE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties sharpenNodeInitProperties =
    {
        NODE_TYPE_SHARPEN,
        nodeStrings[NODE_TYPE_SHARPEN],
        NODE_CAT_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SHARPEN] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties mergeNodeInitProperties =
    {
        NODE_TYPE_MERGE,
        nodeStrings[NODE_TYPE_MERGE],
        NODE_CAT_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_MERGE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties shuffleNodeInitProperties =
    {
        NODE_TYPE_SHUFFLE,
        nodeStrings[NODE_TYPE_SHUFFLE],
        NODE_CAT_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SHUFFLE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties pixelateNodeInitProperties =
    {
        NODE_TYPE_PIXELATE,
        nodeStrings[NODE_TYPE_PIXELATE],
        NODE_CAT_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_PIXELATE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_INT, "Filter Size,0,100,1,5" }
        },
        ":/shaders/pixelate_comp.spv"
    };

    const NodeInitProperties solarizeNodeInitProperties =
    {
        NODE_TYPE_SOLARIZE,
        nodeStrings[NODE_TYPE_SOLARIZE],
        NODE_CAT_FILTER,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_SOLARIZE] },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Red Thresh,0.0,1.0,0.1,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Green Thresh,0.0,1.0,0.1,0.5" },
            { UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE, "Blue Thresh,0.0,1.0,0.1,0.5" }
        },
        ":/shaders/solarize_comp.spv"
    };

    const NodeInitProperties constantNodeInitProperties =
    {
        NODE_TYPE_CONSTANT,
        nodeStrings[NODE_TYPE_CONSTANT],
        NODE_CAT_GENERATE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_CONSTANT] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties noiseNodeInitProperties =
    {
        NODE_TYPE_NOISE,
        nodeStrings[NODE_TYPE_NOISE],
        NODE_CAT_GENERATE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_NOISE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties differenceNodeInitProperties =
    {
        NODE_TYPE_DIFFERENCE,
        nodeStrings[NODE_TYPE_DIFFERENCE],
        NODE_CAT_COLOR,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_DIFFERENCE] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties premultNodeInitProperties =
    {
        NODE_TYPE_PREMULT,
        nodeStrings[NODE_TYPE_PREMULT],
        NODE_CAT_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_PREMULT] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties unpremultNodeInitProperties =
    {
        NODE_TYPE_UNPREMULT,
        nodeStrings[NODE_TYPE_UNPREMULT],
        NODE_CAT_MERGE,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_UNPREMULT] }
        },
        ":/shaders/noop_comp.spv"
    };

    const NodeInitProperties writeNodeInitProperties =
    {
        NODE_TYPE_WRITE,
        nodeStrings[NODE_TYPE_WRITE],
        NODE_CAT_IO,
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, nodeStrings[NODE_TYPE_WRITE] }
        },
        ":/shaders/noop_comp.spv"
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
        throw std::runtime_error("Node type not found.");
    }
}

#endif // NODEDEFINITIONS_H
