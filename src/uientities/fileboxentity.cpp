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

#include "fileboxentity.h"
#include "ui_fileboxentity.h"

#include <iostream>

#include <QFileDialog>
#include <QItemSelectionModel>

FileBoxEntity::FileBoxEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    ui(new Ui::FileBoxEntity)
{
    ui->setupUi(this);

    QStringList slist;

    fileListModel = new QStringListModel(slist, this);
    ui->fileListView->setModel(fileListModel);
    ui->fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

#ifdef QT_DEBUG
    //addEntries(QStringList("C:\\Users\\ryzen\\Cascade\\images\\bay.jpg"));
#endif

    connect(ui->loadButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleLoadButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleDeleteButtonClicked);
    connect(ui->fileListView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, [this] { emit valueChanged(); });
}

QString FileBoxEntity::getCurrentPath()
{
    auto index = ui->fileListView->currentIndex();

    QString itemText = index.data().toString();

    return itemText;
}

void FileBoxEntity::handleLoadButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.png *.tga *.tif *exr)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        addEntries(dialog.selectedFiles());
    }
}

void FileBoxEntity::addEntries(const QStringList& entries)
{
    foreach(QString name, entries)
    {
        fileListModel->insertRow(fileListModel->rowCount());
        QModelIndex index = fileListModel->index(fileListModel->rowCount() - 1, 0);
        fileListModel->setData(index, name);

        ui->fileListView->setCurrentIndex(index);
    }
    emit valueChanged();
}

void FileBoxEntity::deleteCurrentEntry()
{
    QModelIndex index = ui->fileListView->currentIndex();
    int row = index.row();

    fileListModel->removeRows(row, 1);
    if (row > 0)
    {
        index = fileListModel->index(row - 1, 0);
        ui->fileListView->setCurrentIndex(index);
    }
    emit valueChanged();
}

void FileBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &FileBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString FileBoxEntity::getValuesAsString()
{
    QStringList list = fileListModel->stringList();
    QString str = list.join(",");
    str.append(",");
    str.append(QString::number(ui->fileListView->currentIndex().row()));

    return str;
}

void FileBoxEntity::loadPropertyValues(const QString &values)
{
    auto split = values.split(",");
    addEntries(split);
}

void FileBoxEntity::handleDeleteButtonClicked()
{
    deleteCurrentEntry();
}

FileBoxEntity::~FileBoxEntity()
{
    delete ui;
}
