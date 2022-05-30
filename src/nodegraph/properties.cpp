#include "properties.h"

using Cascade::NodeGraph::Properties;

void Properties::put(QString const &name, QVariant const &v)
{
    mValues.insert(name, v);
}


