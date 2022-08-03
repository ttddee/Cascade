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

#include "filespropertyview.h"

#include <QCoreApplication>
#include <QFileDialog>

#include "filespropertymodel.h"

namespace Cascade::Properties
{

FilesPropertyView::FilesPropertyView(QWidget* parent)
    : PropertyView(parent)
{
    mLayout = new QVBoxLayout();
    mLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mLayout);

    mLoadButton = new QPushButton("Load Image(s)...");
    mLoadButton->setMinimumHeight(22);
    mLayout->addWidget(mLoadButton);

    mFileListView = new QListView();
    mLayout->addWidget(mFileListView);

    mDeleteButton = new QPushButton("Delete Image(s)...");
    mDeleteButton->setMinimumHeight(22);
    mLayout->addWidget(mDeleteButton);

    connect(mLoadButton, &QPushButton::clicked,
            this, &FilesPropertyView::handleLoadButtonClicked);
    connect(mDeleteButton, &QPushButton::clicked,
            this, &FilesPropertyView::handleDeleteButtonClicked);
}

void FilesPropertyView::setModel(FilesPropertyModel* model)
{
    mModel = model;
    mFileListView->setModel(mModel->getData()->getFiles());
}

void FilesPropertyView::handleLoadButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(
        tr("Images (*.bmp *.gif *.jpg *.jpeg *.jp2 *.j2k *.j2c *.png *.tga *.tif *.exr)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        mModel->addEntries(dialog.selectedFiles());
    }
}

void FilesPropertyView::handleDeleteButtonClicked()
{
    mModel->removeEntry(mFileListView->currentIndex().row());
}

} // namespace Cascade::Properties
