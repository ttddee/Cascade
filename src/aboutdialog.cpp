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

#include "aboutdialog.h"

#include <QVBoxLayout>
#include <QLabel>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    QPixmap logo(":/design/logo/cascade-logo-full.png");
    QLabel* logoLabel = new QLabel(this);
    logoLabel->setPixmap(logo.scaledToWidth(350, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignHCenter);

    QLabel* textLabel = new QLabel(this);
    textLabel->setText(QString("Version %1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD));
    textLabel->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(15);
    mainLayout->addWidget(logoLabel);
    mainLayout->addWidget(textLabel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("About"));
    setMinimumSize(450, 150);
}
