#include "fileboxentity.h"
#include "ui_fileboxentity.h"

#include <QFileDialog>

FileBoxEntity::FileBoxEntity(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::FileBoxEntity)
{
    ui->setupUi(this);

    connect(ui->loadButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleLoadButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &FileBoxEntity::handleDeleteButtonClicked);

    QStringList slist;
    //slist.append(QString("TEST"));
    fileListModel = new QStringListModel(slist, this);
    ui->fileListView->setModel(fileListModel);
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
