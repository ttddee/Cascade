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
    slist.append(QString("TEST"));
    m_fileListModel = new QStringListModel(slist, this);
    ui->fileListView->setModel(m_fileListModel);
}

void FileBoxEntity::handleLoadButtonClicked()
{
    QFileDialog dialog(nullptr);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Images (*.bmp *.gif *.jpg *.png *.tga *.tif *.cr2 *.nef *.arw)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QCoreApplication::applicationDirPath());
    if (dialog.exec())
    {
        QStringList fileNames = dialog.selectedFiles();
        foreach(QString name, fileNames)
        {
            m_fileListModel->insertRow(m_fileListModel->rowCount());
            QModelIndex index = m_fileListModel->index(m_fileListModel->rowCount() - 1, 0);
            m_fileListModel->setData(index, name);
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
