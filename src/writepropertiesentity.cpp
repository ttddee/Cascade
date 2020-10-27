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

#include "writepropertiesentity.h"
#include "ui_writepropertiesentity.h"

#include <QFileDialog>
#include <QMessageBox>

#include "nodeproperties.h"

WritePropertiesEntity::WritePropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::WritePropertiesEntity)
{
    ui->setupUi(this);

    ui->fileNameEdit->setText(this->fileName);

    connect(ui->fileNameEdit, &QLineEdit::textChanged,
            this, &WritePropertiesEntity::handleFileNametextChanged);
    connect(ui->setFolderButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSetFolderButtonClicked);
    connect(ui->saveImageButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSaveFileButtonClicked);

#ifdef QT_DEBUG
    setFolder("/tmp");
#endif
}

void WritePropertiesEntity::selfConnectToRequestFileSave(NodeProperties *p)
{
    connect(this, &WritePropertiesEntity::requestFileSave,
            p, &NodeProperties::handleFileSaveRequest);
}

QString WritePropertiesEntity::getValuesAsString()
{
    return "";
}

void WritePropertiesEntity::setFileName(const QString& f)
{
    fileName = f;
    updateFileNameLabel();
}

void WritePropertiesEntity::setFolder(const QString& f)
{
    folder = f;
    updateFileNameLabel();

    auto parts = f.split("/");
    ui->folderLabel->setText(parts[parts.size() - 1]);
}

void WritePropertiesEntity::updateFileNameLabel()
{
    QString text = folder + "/" + fileName + ".jpg";
    ui->fileNameLabel->setText(text);
}

void WritePropertiesEntity::handleFileNametextChanged()
{
    setFileName(ui->fileNameEdit->text());
}

void WritePropertiesEntity::handleSetFolderButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setViewMode(QFileDialog::Detail);
    auto f = dialog.getExistingDirectory();
    if (!f.isEmpty())
    {
        setFolder(f);
    }
}

void WritePropertiesEntity::handleSaveFileButtonClicked()
{
    if (folder != "")
    {
        emit requestFileSave(ui->fileNameLabel->text());
    }
    else
    {
        QMessageBox messageBox;
        messageBox.information(0,"No folder set","Please select a folder.");
    }
}

WritePropertiesEntity::~WritePropertiesEntity()
{
    delete ui;
}
