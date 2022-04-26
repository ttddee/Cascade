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

#include "preferencesdialog.h"

#include <QVBoxLayout>
#include <QJsonObject>
#include <QHeaderView>

#include "preferencesmanager.h"
#include "log.h"

namespace Cascade {

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent)
{
    mTabWidget = new QTabWidget;
    //mTabWidget->addTab(new QWidget(), tr("General"));

    loadKeys();

    mButtonBox = new QDialogButtonBox(
                QDialogButtonBox::Save |
                QDialogButtonBox::Cancel);

    connect(mButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(mButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mTabWidget);
    mainLayout->addWidget(mButtonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Preferences"));
    setMinimumSize(600, 700);
}

void PreferencesDialog::loadKeys()
{
    auto prefsManager = &PreferencesManager::getInstance();
    auto keyCategories = prefsManager->getKeys();

    mKeysWidget = new QTableWidget(0, 2);
    QStringList labels({ "Function", "Key" });
    mKeysWidget->setHorizontalHeaderLabels(labels);
    mKeysWidget->horizontalHeader()->setStretchLastSection(true);
    mKeysWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    mKeysWidget->setShowGrid(false);
    mKeysWidget->verticalHeader()->hide();
    mKeysWidget->setColumnWidth(0, 200);

    QFont bold;
    bold.setBold(true);

    for (auto& category : keyCategories)
    {
        mKeysWidget->insertRow(mKeysWidget->rowCount());
        QTableWidgetItem* heading = new QTableWidgetItem(category.name.toUpper());
        heading->setFont(bold);
        mKeysWidget->setItem(mKeysWidget->rowCount() - 1, 0, heading);
        for (auto& pair : category.keys)
        {
            mKeysWidget->insertRow(mKeysWidget->rowCount());
            QTableWidgetItem* k = new QTableWidgetItem(pair.first);
            mKeysWidget->setItem(mKeysWidget->rowCount() - 1, 0, k);
            QTableWidgetItem* v = new QTableWidgetItem(pair.second);
            mKeysWidget->setItem(mKeysWidget->rowCount() - 1, 1, v);
        }
        mKeysWidget->insertRow(mKeysWidget->rowCount());
    }

    mTabWidget->addTab(mKeysWidget, tr("Keys"));
}

} // namespace Cascade
