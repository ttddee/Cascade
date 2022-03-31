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

#include "writepropertiesentity.h"
#include "ui_writepropertiesentity.h"

#include <QFileDialog>

#include "../popupmessages.h"

namespace Cascade {

WritePropertiesEntity::WritePropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    ui(new Ui::WritePropertiesEntity)
{
    ui->setupUi(this);

    ui->fileNameEdit->setText(this->fileName);

    foreach(auto& t, filetypes)
    {
        ui->fileTypeBox->addItem(t);
    }

    jpegCompressionSlider = new CsSliderBoxEntity(
                UI_ELEMENT_TYPE_SLIDER_BOX_INT,
                this);
    jpegCompressionSlider->setName("Quality");
    jpegCompressionSlider->setMinMaxStepValue(1, 100, 1, 100);
    ui->verticalLayout->insertWidget(4, jpegCompressionSlider);

    connect(ui->fileNameEdit, &QLineEdit::textChanged,
            this, &WritePropertiesEntity::handleFileNametextChanged);
    connect(ui->setFolderButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSetFolderButtonClicked);
    connect(ui->saveImageButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSaveFileButtonClicked);
    connect(ui->fileTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WritePropertiesEntity::handleFileTypeChanged);
    connect(jpegCompressionSlider, &CsSliderBoxEntity::valueChanged,
            this, &WritePropertiesEntity::updateAttributes);

    updateAttributes();

#ifdef QT_DEBUG
    //setFolder("/tmp");
#endif
}

void WritePropertiesEntity::selfConnectToRequestFileSave(NodeProperties *p)
{
    connect(this, &WritePropertiesEntity::requestFileSave,
            p, &NodeProperties::handleFileSaveRequest);
}

QString WritePropertiesEntity::getValuesAsString()
{
    QString str = folder;
    str.append(",");
    str.append(QString::number(ui->fileTypeBox->currentIndex()));
    str.append(",");
    str.append(fileName),
    str.append(","),
    str.append(QString::number(ui->batchCheckBox->isChecked()));
    str.append(","),
    str.append(jpegCompressionSlider->getValuesAsString());

    return str;
}

void WritePropertiesEntity::loadPropertyValues(const QString &values)
{
    auto split = values.split(",");
    QFileInfo checkPath(split[0]);
    if (checkPath.exists())
        setFolder(split[0]);
    else
        setFolder("");
    ui->fileTypeBox->setCurrentIndex(split[1].toInt());
    setFileName(split[2]);
    ui->batchCheckBox->setChecked(split[3].toInt());
    jpegCompressionSlider->setMinMaxStepValue(1, 100, 1, split[4].toInt());
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
    QString text = fileName + "." + ui->fileTypeBox->currentText();
    ui->fileNameLabel->setText(text);
}

void WritePropertiesEntity::updateAttributes()
{
    attributes.clear();
    if (ui->fileTypeBox->currentText() == "jpg")
    {
        attributes.insert("Compression", "jpeg:" +
            jpegCompressionSlider->getValuesAsString().toStdString());
    }
}

void WritePropertiesEntity::hideAllAttributeElements()
{
    jpegCompressionSlider->setVisible(false);
}

void WritePropertiesEntity::handleFileNametextChanged()
{
    setFileName(ui->fileNameEdit->text());
}

void WritePropertiesEntity::handleFileTypeChanged()
{
    this->hideAllAttributeElements();
    if (ui->fileTypeBox->currentText() == "jpg")
    {
        jpegCompressionSlider->setVisible(true);
    }

    updateFileNameLabel();
    updateAttributes();
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
        emit requestFileSave(
                    folder + "/" + fileName,
                    ui->fileTypeBox->currentText(),
                    attributes,
                    ui->batchCheckBox->isChecked());
    }
    else
    {
        executeMessageBox(MESSAGEBOX_NO_FOLDER_SET);
    }
}

WritePropertiesEntity::~WritePropertiesEntity()
{
    delete ui;
}

} // namespace Cascade
