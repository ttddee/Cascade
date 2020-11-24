/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#include "folderboxentity.h"
#include "ui_folderboxentity.h"

#include <QFileDialog>

#include "nodeproperties.h"

FolderBoxEntity::FolderBoxEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::FolderBoxEntity)
{
    ui->setupUi(this);

    ui->lineEdit->setText(QCoreApplication::applicationDirPath());

    connect(ui->pushButton, &QPushButton::clicked,
            this, &FolderBoxEntity::handleFolderButtonClicked);
}

void FolderBoxEntity::setName(const QString &name)
{
    ui->label->setText(name);
}

void FolderBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &FolderBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString FolderBoxEntity::getValuesAsString()
{
    return "\"" +  ui->lineEdit->text() + "\"";
}

void FolderBoxEntity::handleFolderButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        ui->lineEdit->setText(dialog.selectedFiles().at(0));
    }
}

FolderBoxEntity::~FolderBoxEntity()
{
    delete ui;
}
