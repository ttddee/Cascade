#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "nodedefinitions.h"
#include "uientity.h"

using namespace Cascade;

class NodeProperties : public QWidget
{
    Q_OBJECT

public:
    explicit NodeProperties(const NodeType t, QWidget *parent = nullptr);

    std::vector<UiEntity*> widgets;

private:
    const NodeType nodeType;
    QVBoxLayout* layout;

public slots:
    void handleSomeValueChanged();
};

#endif // NODEPROPERTIES_H
