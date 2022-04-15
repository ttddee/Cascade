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
    tabWidget = new QTabWidget;
    tabWidget->addTab(new QWidget(), tr("General"));

    loadKeys();

    buttonBox = new QDialogButtonBox(
                QDialogButtonBox::Save |
                QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Preferences"));
    setMinimumSize(600, 500);
}

void PreferencesDialog::loadKeys()
{
    auto prefsManager = &PreferencesManager::getInstance();
    auto keyCategories = prefsManager->getKeys();

    keysWidget = new QTableWidget(0, 2);
    QStringList labels({ "Function", "Key" });
    keysWidget->setHorizontalHeaderLabels(labels);
    keysWidget->horizontalHeader()->setStretchLastSection(true);
    keysWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    keysWidget->setShowGrid(false);
    keysWidget->verticalHeader()->hide();
    keysWidget->setColumnWidth(0, 200);

    QFont bold;
    bold.setBold(true);

    for (auto& category : keyCategories)
    {
        keysWidget->insertRow(keysWidget->rowCount());
        QTableWidgetItem* heading = new QTableWidgetItem(category.name.toUpper());
        heading->setFont(bold);
        keysWidget->setItem(keysWidget->rowCount() - 1, 0, heading);
        for (auto& pair : category.keys)
        {
            keysWidget->insertRow(keysWidget->rowCount());
            QTableWidgetItem* k = new QTableWidgetItem(pair.first);
            keysWidget->setItem(keysWidget->rowCount() - 1, 0, k);
            QTableWidgetItem* v = new QTableWidgetItem(pair.second);
            keysWidget->setItem(keysWidget->rowCount() - 1, 1, v);
        }
        keysWidget->insertRow(keysWidget->rowCount());
    }

    tabWidget->addTab(keysWidget, tr("Keys"));
}

} // namespace Cascade
