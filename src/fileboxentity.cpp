#include "fileboxentity.h"
#include "ui_fileboxentity.h"

#include <iostream>

#include <QFileDialog>
#include <QItemSelectionModel>

#include "nodeproperties.h"

FileBoxEntity::FileBoxEntity(UIElementType et, QWidget *parent)
    : UiEntity(et, parent),
    ui(new Ui::FileBoxEntity)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    QStringList slist;
    slist.append(QString("../../images/bay.jpg"));
#endif

    fileListModel = new QStringListModel(slist, this);
    ui->fileListView->setModel(fileListModel);
    ui->fileListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Temporary
    QModelIndex index = fileListModel->index(fileListModel->rowCount() - 1, 0);
    ui->fileListView->setCurrentIndex(index);

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
        emit valueChanged();
    }
}

void FileBoxEntity::selfConnectToValueChanged(NodeProperties *p)
{
    connect(this, &FileBoxEntity::valueChanged,
            p, [p]{p->handleSomeValueChanged();});
}

QString FileBoxEntity::getValuesAsString()
{
    return getCurrentPath();
}

void FileBoxEntity::handleDeleteButtonClicked()
{

}

FileBoxEntity::~FileBoxEntity()
{
    delete ui;
}
