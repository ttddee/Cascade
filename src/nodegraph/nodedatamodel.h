#pragma once


#include <QtWidgets/QWidget>

#include "porttype.h"
#include "nodedata.h"
#include "serializable.h"
#include "nodegeometry.h"
#include "nodestyle.h"
#include "nodepainterdelegate.h"
#include "memory.h"

namespace Cascade::NodeGraph
{

enum class NodeValidationState
{
    Valid,
    Warning,
    Error
};

class Connection;

class StyleCollection;

class NodeDataModel :
    public QObject,
    public Serializable
{
    Q_OBJECT

public:
    NodeDataModel();

    virtual ~NodeDataModel() = default;

    /// Caption is used in GUI
    virtual QString caption() const
    {
        return mData.mCaption;
    };

    virtual QString name() const
    {
        return mData.mName;
    }

    /// Port caption is used in GUI to label individual ports
    virtual QString portCaption(PortType portType, PortIndex portIndex) const
    {
        if (portType == PortType::In)
            return mData.mInPorts.at(portIndex);
        else if (portType == PortType::Out)
            return mData.mOutPorts.at(portIndex);
        else return QString();
    }

public:
    QJsonObject save() const override;

public:
    virtual unsigned int nPorts(PortType portType) const = 0;

    virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

public:
    enum class ConnectionPolicy
    {
        One,
        Many,
    };

    virtual ConnectionPolicy portOutConnectionPolicy(PortIndex) const
    {
        return ConnectionPolicy::Many;
    }

    virtual ConnectionPolicy portInConnectionPolicy(PortIndex) const
    {
        return ConnectionPolicy::One;
    }

    NodeStyle const& nodeStyle() const;

    void setNodeStyle(NodeStyle const& style);

public:
    /// Triggers the algorithm
    virtual void setInData(
        std::shared_ptr<NodeData> nodeData,
        PortIndex port) = 0;

    // Use this if portInConnectionPolicy returns ConnectionPolicy::Many
    virtual void setInData(
        std::shared_ptr<NodeData> nodeData,
        PortIndex port,
        const QUuid& connectionId)
    {
        Q_UNUSED(connectionId);
        setInData(nodeData, port);
    }

    virtual std::shared_ptr<NodeData> outData(PortIndex port) = 0;

    virtual QWidget* embeddedWidget() = 0;

    virtual bool resizable() const
    {
        return false;
    }

    virtual NodeValidationState validationState() const
    {
        return NodeValidationState::Valid;
    }

    virtual QString validationMessage() const
    {
        return QString("");
    }

    virtual NodePainterDelegate* painterDelegate() const
    {
        return nullptr;
    }

public Q_SLOTS:
    virtual void inputConnectionCreated(Connection const&)
    {
    }

    virtual void inputConnectionDeleted(Connection const&)
    {
    }

    virtual void outputConnectionCreated(Connection const&)
    {
    }

    virtual void outputConnectionDeleted(Connection const&)
    {
    }

Q_SIGNALS:
    void dataUpdated(PortIndex index);

    void dataInvalidated(PortIndex index);

    void computingStarted();

    void computingFinished();

    void embeddedWidgetSizeUpdated();

protected:
    NodeData mData;

private:
    NodeStyle mNodeStyle;

};
}
