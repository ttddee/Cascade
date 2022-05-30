/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
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

#ifndef PROPERTIESVIEW_H
#define PROPERTIESVIEW_H

#include <QWidget>

#include <QSlider>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

namespace Cascade::Properties {

class PropertyView : public QWidget
{
    Q_OBJECT

public:
    PropertyView(QWidget *parent = nullptr)
        : QWidget(parent)
        {}

signals:
    void valueChanged();

};


class IntPropertyView : public PropertyView
{
    Q_OBJECT

public:
    IntPropertyView(QWidget *parent = nullptr)
        : PropertyView(parent)
    {
        setMaximumHeight(22);

        mLayout = new QGridLayout();
        mLayout->setVerticalSpacing(0);
        mLayout->setContentsMargins(0, 0, 0, 0);
        setLayout(mLayout);

        mSlider = new QSlider(Qt::Horizontal, this);
        mLayout->addWidget(mSlider);

        mNameLabel = new QLabel(this);
        mNameLabel->setText("None");
        mNameLabel->setObjectName("nameLabel");

        mLayout->addWidget(mNameLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);
        mValueBox = new QSpinBox(this);
        mValueBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mLayout->addWidget(mValueBox, 0, 0, Qt::AlignHCenter | Qt::AlignVCenter);

    }

private:
    //bool eventFilter(QObject* watched, QEvent* event) override;

    QGridLayout* mLayout;
    QSlider* mSlider;
    QLabel* mNameLabel;
    QSpinBox* mValueBox;

};

} // namespace Cascade::Properties

#endif // PROPERTIESVIEW_H
