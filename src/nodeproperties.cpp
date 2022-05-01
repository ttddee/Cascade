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

#include "nodeproperties.h"

#include <QLabel>

#include "nodebase.h"
#include "projectmanager.h"
#include "uientities/cssliderboxentity.h"
#include "uientities/fileboxentity.h"
#include "propertiesheading.h"
#include "uientities/colorbuttonentity.h"
#include "uientities/writepropertiesentity.h"
#include "uientities/spinboxentity.h"
#include "uientities/comboboxentity.h"
#include "uientities/channelselectentity.h"
#include "uientities/colorpropertiesentity.h"
#include "uientities/sizeboxentity.h"
#include "uientities/textboxentity.h"
#include "uientities/checkboxentity.h"
#include "uientities/textbrowserentity.h"
#include "uientities/separatorentity.h"
#include "uientities/lineeditentity.h"
#include "uientities/folderboxentity.h"
#include "uientities/uientity.h"
#include "uientities/resizepropertiesentity.h"
#include "uientities/codeeditorentity.h"

namespace Cascade {

NodeProperties::NodeProperties(
        const NodeType t,
        NodeBase* parentNode,
        const NodeInitProperties& initProps,
        QWidget *parent)
        : QWidget(parent),
          mNodeType(t),
          mParentNode(parentNode)
{
    mLayout = new QVBoxLayout();
    mLayout->setAlignment(Qt::AlignTop);
    mLayout->setContentsMargins(QMargins(0,0,0,0));
    this->setLayout(mLayout);

    ProjectManager* pm = &ProjectManager::getInstance();
    connect(this, &NodeProperties::projectIsDirty,
            pm, &ProjectManager::handleProjectIsDirty);
    connect(this, &NodeProperties::requestNodeUpdate,
            parentNode, &NodeBase::handleRequestNodeUpdate);

    foreach (auto& elem, initProps.uiElements)
    {
        if(elem.first == UIElementType::ePropertiesHeading)
        {
            PropertiesHeading* item = new PropertiesHeading(
                        elem.second.toUpper(),
                        this);
            mLayout->addWidget(item);
        }
        else if (elem.first == UIElementType::eSpinBox)
        {
            SpinBoxEntity* item =
                    new SpinBoxEntity(
                        UIElementType::eSpinBox,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eFileBox)
        {
            FileBoxEntity* item = new FileBoxEntity(
                        UIElementType::eFileBox,
                        this);
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eColorButton)
        {
            ColorButtonEntity* item = new ColorButtonEntity(
                        UIElementType::eColorButton,
                        this);
            item->selfConnectToValueChanged(this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            QColor color = QColor(
                        parts.at(1).toFloat() * 255.0,
                        parts.at(2).toFloat() * 255.0,
                        parts.at(3).toFloat() * 255.0,
                        parts.at(4).toFloat() * 255.0);
            item->setColor(color);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eWriteProperties)
        {
            WritePropertiesEntity* item = new WritePropertiesEntity(
                        UIElementType::eWriteProperties,
                        this);
            item->selfConnectToRequestFileSave(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eComboBox)
        {
            ComboBoxEntity* item = new ComboBoxEntity(
                        UIElementType::eComboBox,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts[0]);
            std::vector<QString> options;
            for (int i = 1; i < parts.size() - 1; ++i)
            {
                options.push_back(parts[i]);
            }
            item->setOptions(options, parts.last().toInt());
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eChannelSelect)
        {
            ChannelSelectEntity* item = new ChannelSelectEntity(
                        UIElementType::eChannelSelect,
                        this);
            auto parts = elem.second.split(",");
            if (parts.at(0).toInt() == 1)
            {
                item->hideAlphaChannel();
            }
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eSliderBoxDouble)
        {
            CsSliderBoxEntity* item =
                    new CsSliderBoxEntity(
                        UIElementType::eSliderBoxDouble,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toDouble(),
                        parts.at(2).toDouble(),
                        parts.at(3).toDouble(),
                        parts.at(4).toDouble());
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eSliderBoxInt)
        {
            CsSliderBoxEntity* item =
                    new CsSliderBoxEntity(
                        UIElementType::eSliderBoxInt,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setMinMaxStepValue(
                        parts.at(1).toInt(),
                        parts.at(2).toInt(),
                        parts.at(3).toInt(),
                        parts.at(4).toInt());
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eColorProperties)
        {
            ColorPropertiesEntity* item = new ColorPropertiesEntity(
                        UIElementType::eColorProperties,
                        this);
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eSizeBox)
        {
            SizeBoxEntity* item = new SizeBoxEntity(
                        UIElementType::eSizeBox,
                        this);
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eTextBox)
        {
            TextBoxEntity* item = new TextBoxEntity(
                        UIElementType::eTextBox,
                        this);
            item->setText(elem.second);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eCheckBox)
        {
            CheckBoxEntity* item = new CheckBoxEntity(
                        UIElementType::eCheckBox,
                        this);
            item->selfConnectToValueChanged(this);
            auto parts = elem.second.split(",");
            item->setName(parts.at(0));
            item->setChecked(parts.at(1).toInt());
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eTextBrowser)
        {
            TextBrowserEntity* item = new TextBrowserEntity(
                        UIElementType::eTextBox,
                        this);
            item->setText(elem.second);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eSeparator)
        {
            SeparatorEntity* item = new SeparatorEntity(
                        UIElementType::eSeparator,
                        this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eLineEdit)
        {
            LineEditEntity* item = new LineEditEntity(
                        UIElementType::eLineEdit,
                        this);
            auto parts = elem.second.split(",");
            item->setName(parts[0]);
            item->setText(parts[1]);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eFolderBox)
        {
            FolderBoxEntity* item = new FolderBoxEntity(
                        UIElementType::eFolderBox,
                        this);
            item->setName(elem.second);
            item->selfConnectToValueChanged(this);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
        else if (elem.first == UIElementType::eResizeProperties)
        {
            ResizePropertiesEntity* item = new ResizePropertiesEntity(
                        UIElementType::eSizeBox,
                        this);
            item->selfConnectToValueChanged(this);
            item->setParentNode(parentNode);
            mLayout->addWidget(item);
            mWidgets.push_back(item);

            connect(parentNode, &NodeBase::nodeRequestUpdate,
                    item, &ResizePropertiesEntity::handleNodeRequestUpdate);
        }
        else if (elem.first == UIElementType::eCodeEditor)
        {
            CodeEditorEntity* item = new CodeEditorEntity(
                        UIElementType::eCodeEditor,
                        this);
            item->selfConnectToValueChanged(this);
            item->setParentNode(parentNode);
            mLayout->addWidget(item);
            mWidgets.push_back(item);
        }
    }
}

void NodeProperties::loadNodePropertyValues(const QMap<int, QString> &values)
{
    for (int i = 0; i < values.size(); i++)
    {
        mWidgets[i]->loadPropertyValues(values[i]);
    }
}

void NodeProperties::handleSomeValueChanged()
{
    emit requestNodeUpdate();

    emit projectIsDirty();
}

void NodeProperties::handleFileSaveRequest(
        const QString& path,
        const QString& fileType,
        const QMap<std::string, std::string>& attributes,
        const bool batchRender)
{
    emit mParentNode->nodeRequestFileSave(mParentNode, path, fileType, attributes, batchRender);
}

int NodeProperties::getNumImages()
{
    auto entity = static_cast<FileBoxEntity*>(mWidgets.at(0));

    return entity->getNumImages();
}

void NodeProperties::switchToFirstImage()
{
    auto entity = static_cast<FileBoxEntity*>(mWidgets.at(0));

    entity->switchToFirstImage();
}

void NodeProperties::switchToNextImage()
{
    auto entity = static_cast<FileBoxEntity*>(mWidgets.at(0));

    entity->switchToNextImage();
}

} // namespace Cascade
