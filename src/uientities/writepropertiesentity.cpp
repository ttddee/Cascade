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
#include "../global.h"

namespace Cascade {

WritePropertiesEntity::WritePropertiesEntity(UIElementType et, QWidget *parent) :
    UiEntity(et, parent),
    mUi(new Ui::WritePropertiesEntity)
{
    mUi->setupUi(this);

    mUi->fileNameEdit->setText(this->mFileName);

    mUi->batchCheckBox->setToolTip(Config::sBatchRenderCheckBoxTooltip);

    foreach(auto& t, mFiletypes)
    {
        mUi->fileTypeBox->addItem(t);
    }

    mJpegCompressionSlider = new CsSliderBoxEntity(
                UIElementType::eSliderBoxInt,
                this);
    mJpegCompressionSlider->setName("Quality");
    mJpegCompressionSlider->setMinMaxStepValue(1, 100, 1, 100);
    mUi->verticalLayout->insertWidget(4, mJpegCompressionSlider);

    connect(mUi->fileNameEdit, &QLineEdit::textChanged,
            this, &WritePropertiesEntity::handleFileNametextChanged);
    connect(mUi->setFolderButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSetFolderButtonClicked);
    connect(mUi->saveImageButton, &QPushButton::clicked,
            this, &WritePropertiesEntity::handleSaveFileButtonClicked);
    connect(mUi->fileTypeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &WritePropertiesEntity::handleFileTypeChanged);
    connect(mJpegCompressionSlider, &CsSliderBoxEntity::valueChanged,
            this, &WritePropertiesEntity::updateAttributes);

    updateAttributes();

#ifdef QT_DEBUG
    //setFolder("/tmp");
#endif
}

void WritePropertiesEntity::selfConnectToRequestFileSave(NodeProperties *p)
{
//    connect(this, &WritePropertiesEntity::requestFileSave,
//            p, &NodeProperties::handleFileSaveRequest);
}

QString WritePropertiesEntity::getValuesAsString()
{
    QString str = mFolder;
    str.append(",");
    str.append(QString::number(mUi->fileTypeBox->currentIndex()));
    str.append(",");
    str.append(mFileName),
    str.append(","),
    str.append(QString::number(mUi->batchCheckBox->isChecked()));
    str.append(","),
    str.append(mJpegCompressionSlider->getValuesAsString());

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
    mUi->fileTypeBox->setCurrentIndex(split[1].toInt());
    setFileName(split[2]);
    mUi->batchCheckBox->setChecked(split[3].toInt());
    mJpegCompressionSlider->setMinMaxStepValue(1, 100, 1, split[4].toInt());
}

void WritePropertiesEntity::setFileName(const QString& f)
{
    mFileName = f;
    updateFileNameLabel();
}

void WritePropertiesEntity::setFolder(const QString& f)
{
    mFolder = f;
    updateFileNameLabel();

    auto parts = f.split("/");
    mUi->folderLabel->setText(parts[parts.size() - 1]);
}

void WritePropertiesEntity::updateFileNameLabel()
{
    QString text = mFileName + "." + mUi->fileTypeBox->currentText();
    mUi->fileNameLabel->setText(text);
}

void WritePropertiesEntity::updateAttributes()
{
    mAttributes.clear();
    if (mUi->fileTypeBox->currentText() == "jpg")
    {
        mAttributes.insert("Compression", "jpeg:" +
            mJpegCompressionSlider->getValuesAsString().toStdString());
    }
}

void WritePropertiesEntity::hideAllAttributeElements()
{
    mJpegCompressionSlider->setVisible(false);
}

void WritePropertiesEntity::handleFileNametextChanged()
{
    setFileName(mUi->fileNameEdit->text());
}

void WritePropertiesEntity::handleFileTypeChanged()
{
    this->hideAllAttributeElements();
    if (mUi->fileTypeBox->currentText() == "jpg")
    {
        mJpegCompressionSlider->setVisible(true);
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
    if (mFolder != "")
    {
        emit requestFileSave(
                    mFolder + "/" + mFileName,
                    mUi->fileTypeBox->currentText(),
                    mAttributes,
                    mUi->batchCheckBox->isChecked());
    }
    else
    {
        executeMessageBox(MESSAGEBOX_NO_FOLDER_SET);
    }
}

WritePropertiesEntity::~WritePropertiesEntity()
{
    delete mUi;
}

} // namespace Cascade
