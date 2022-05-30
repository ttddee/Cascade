#pragma once

#include <unordered_map>

#include <QtCore/QVariantMap>

namespace Cascade::NodeGraph
{

class Properties
{
public:

    void put(QString const &name, QVariant const &v);

    template <typename T>
    bool get(QString name, T* v) const
    {
        QVariant const &var = mValues[name];

        if (var.canConvert<T>())
        {
            *v = mValues[name].value<T>();

            return true;
        }

        return false;
    }

    QVariantMap const& values() const
    {
        return mValues;
    }

    QVariantMap& values()
    {
        return mValues;
    }

private:

    QVariantMap mValues;
};
}
