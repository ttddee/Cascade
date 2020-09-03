#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "nodedefinitions.h"

using namespace Cascade;

class NodeProperties : public QWidget
{
    Q_OBJECT

public:
    explicit NodeProperties(const NodeType t, QWidget *parent = nullptr);

private:
    const NodeType nodeType;
    QVBoxLayout* layout;
};

#endif // NODEPROPERTIES_H
