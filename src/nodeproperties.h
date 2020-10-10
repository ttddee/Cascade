#ifndef NODEPROPERTIES_H
#define NODEPROPERTIES_H

#include <QObject>
#include <QWidget>
#include <QLayout>

#include "nodedefinitions.h"
#include "uientity.h"

using namespace Cascade;

class NodeBase;

class NodeProperties : public QWidget
{
    Q_OBJECT

public:
    explicit NodeProperties(
            const NodeType t,
            NodeBase* parentNode,
            QWidget *parent = nullptr);

    std::vector<UiEntity*> widgets;

private:
    const NodeType nodeType;
    QVBoxLayout* layout;
    NodeBase* parentNode;

public slots:
    void handleSomeValueChanged();
    void handleFileSaveRequest(const QString& path);
};

#endif // NODEPROPERTIES_H
