#include "fileboxentity.h"
#include "ui_fileboxentity.h"

#include <iostream>

#include <QFileDialog>

FileBoxEntity::FileBoxEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    ui(new Ui::FileBoxEntity)
{
    ui->setupUi(this);

    connect(ui->loadButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleLoadButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleDeleteButtonClicked);

    QStringList slist;
    slist.append(QString("../../images/bay.jpg")); // TODO: Temporary
    fileListModel = new QStringListModel(slist, this);
    ui->fileListView->setModel(fileListModel);
    ui->fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
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
    //dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.png *.tga *.tif *.cr2 *.nef *.arw)"));
    dialog.setNameFilter(tr("Images (*.gif *.jpg *.png)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        foreach(QString name, fileNames)
        {
            fileListModel->insertRow(fileListModel->rowCount());
            QModelIndex index = fileListModel->index(fileListModel->rowCount() - 1, 0);
            fileListModel->setData(index, name);

            ui->fileListView->setCurrentIndex(index);
        }
    }
}

void FileBoxEntity::handleDeleteButtonClicked()
{

}

FileBoxEntity::~FileBoxEntity()
{
    delete ui;
}
