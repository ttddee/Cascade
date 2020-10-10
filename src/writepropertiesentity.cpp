#include "writepropertiesentity.h"
#include "ui_writepropertiesentity.h"

#include <QFileDialog>

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
}

WritePropertiesEntity::~WritePropertiesEntity()
{
    delete ui;
}
