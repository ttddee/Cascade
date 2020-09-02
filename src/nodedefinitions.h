#ifndef NODEDEFINITIONS_H
#define NODEDEFINITIONS_H

#include <QMap>

namespace Cascade
{
    // Global node types
    enum NodeType
    {
        NODE_TYPE_READ,
        NODE_TYPE_TEST,
        NODE_TYPE_MAX
    };

    static const QMap<NodeType, QString> nodeStrings =
    {
        { NODE_TYPE_READ, "Read" },
        { NODE_TYPE_TEST, "Test" }
    };

    // Global input types
    enum NodeInputType
    {
        NODE_INPUT_TYPE_NONE,
        NODE_INPUT_TYPE_RGB_FRONT,
        NODE_INPUT_TYPE_RGB_BACK,
        NODE_INPUT_TYPE_ALPHA
    };

    // Global output types
    enum NodeOutputType
    {
        NODE_OUTPUT_TYPE_RGB
    };

    // Global UI element types
    enum UIElementType
    {
        UI_ELEMENT_TYPE_PROPERTIES_HEADING,
        UI_ELEMENT_TYPE_SLIDERSPIN_INT,
        UI_ELEMENT_TYPE_SLIDERSPIN_DOUBLE,
        UI_ELEMENT_TYPE_FILEBOX
    };

    /////////////////////////////////////

    struct NodeInitProperties
    {
        NodeType nodeType;
        QString title;
        std::vector<NodeInputType> nodeInputs;
        std::vector<NodeOutputType> nodeOutputs;
        QMap<UIElementType, std::string> uiElements;
    };

    /////////////////////////////////////

    const NodeInitProperties readNodeInitProperties =
    {
        NODE_TYPE_READ,
        "Read",
        { NODE_INPUT_TYPE_RGB_BACK },
        { NODE_OUTPUT_TYPE_RGB },
        {
            { UI_ELEMENT_TYPE_PROPERTIES_HEADING, "Read"},
            { UI_ELEMENT_TYPE_FILEBOX, ""}
        }
    };
}

#endif // NODEDEFINITIONS_H
