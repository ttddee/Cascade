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

#include <QFileDialog>

namespace Cascade {

ListItem::ListItem()
{

}

bool ListItem::valid()
{
    return this->isValid;
}

void ListItem::setValid(const bool b)
{
    this->isValid = b;
}

ListItem::~ListItem()
{

}

FileBoxEntity::FileBoxEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    mUi(new Ui::FileBoxEntity)
{
    mUi->setupUi(this);

#ifdef QT_DEBUG
    addEntries(QStringList("C:\\Users\\ryzen\\Cascade\\images\\bay.jpg"));
#endif

    connect(mUi->loadButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleLoadButtonClicked);
    connect(mUi->deleteButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleDeleteButtonClicked);
    connect(mUi->fileListWidget, &QListWidget::currentItemChanged,
            this, &FileBoxEntity::valueChanged);
}

void FileBoxEntity::handleLoadButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.jpeg *.jp2 *.j2k *.j2c *.png *.tga *.tif *exr)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        addEntries(dialog.selectedFiles());
    }
}

void FileBoxEntity::addEntries(const QStringList& entries)
{
    foreach(QString path, entries)
    {
        ListItem *item = new ListItem();
        item->setText(path);
        int idx = mUi->fileListWidget->count();
        mUi->fileListWidget->insertItem(idx, item);
        mUi->fileListWidget->setCurrentRow(idx);

        if (!fileExists(path))
        {
            item->setValid(false);
            item->setForeground(QBrush(QColor(229, 70, 61)));
        }
    }
}

void FileBoxEntity::deleteCurrentEntry()
{
    int row = mUi->fileListWidget->currentRow();
    delete mUi->fileListWidget->takeItem(row);

    if (row > 0)
    {
        mUi->fileListWidget->setCurrentRow(row - 1);
    }
    emit valueChanged();
}

void FileBoxEntity::selfConnectToValueChanged( [[maybe_unused]] NodeProperties *p)
{
//    connect(this, &FileBoxEntity::valueChanged,
//            p, [p]{p->handleSomeValueChanged();});
}

QString FileBoxEntity::getValuesAsString()
{
    QString str;
    for (int i = 0; i < mUi->fileListWidget->count(); ++i)
    {
        str.append(mUi->fileListWidget->item(i)->text());
        str.append(",");
    }
    if (mUi->fileListWidget->count() > 0)
        str.append(QString::number(mUi->fileListWidget->currentRow()));

    return str;
}

void FileBoxEntity::loadPropertyValues(const QString &values)
{
    auto split = values.split(",");
    split.removeLast();
    addEntries(split);
}

bool FileBoxEntity::fileExists(const QString& path)
{
    QFileInfo checkFile(path);
    return checkFile.exists() && checkFile.isFile();
}

int FileBoxEntity::getNumImages()
{
    return mUi->fileListWidget->count();
}

void FileBoxEntity::switchToFirstImage()
{
    mUi->fileListWidget->setCurrentRow(0);
}

void FileBoxEntity::switchToNextImage()
{
    int count = mUi->fileListWidget->count();
    if (mUi->fileListWidget->currentRow() < (count - 1))
        mUi->fileListWidget->setCurrentRow(mUi->fileListWidget->currentRow() + 1);
}

void FileBoxEntity::handleDeleteButtonClicked()
{
    deleteCurrentEntry();
}

FileBoxEntity::~FileBoxEntity()
{
    delete mUi;
}

} // namespace Cascade
