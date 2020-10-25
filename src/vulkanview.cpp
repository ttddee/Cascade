/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2020 The Cascade developers
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

#include "vulkanview.h"

#include <QApplication>
#include <QHBoxLayout>

#include "csmessagebox.h"

VulkanView::VulkanView(QWidget *parent) : QWidget(parent)
{
    qDebug("Creating Vulkan instance");

    // Set up validation layers
    instance.setLayers(QByteArrayList()
                       << "VK_LAYER_GOOGLE_threading"
                       << "VK_LAYER_LUNARG_parameter_validation"
                       << "VK_LAYER_LUNARG_object_tracker"
                       << "VK_LAYER_LUNARG_core_validation"
                       << "VK_LAYER_LUNARG_image"
                       << "VK_LAYER_LUNARG_swapchain"
                       << "VK_LAYER_GOOGLE_unique_objects");

    if (!instance.create())
    {
        CsMessageBox messageBox;
        messageBox.setWindowTitle("Error");
        messageBox.setText("**Failed to initialize Vulkan.**\n"
                           "\n"
                           "It could be that your GPU is not supported or your driver needs to be updated.\n"
                           "You can find a list of supported GPUs at **vulkan.gpuinfo.org**\n"
                           "\n"
                           "If you want to help out the developers, please send the file **cascade.log** "
                           "and the make and model of your GPU to \n"
                           "\n"
                           "**cascadeapp @ protonmail.com**\n"
                           "\n"
                           "Or open an issue on: **github.com/ttddee/Cascade**"
                           );
        messageBox.exec();

        qFatal("Failed to create Vulkan instance. Error code: %d", instance.errorCode());
    }

    // Create a VulkanWindow
    vulkanWindow = new VulkanWindow();
    vulkanWindow->setVulkanInstance(&instance);

    vulkanWindow->setPreferredColorFormats(QVector<VkFormat>() << VK_FORMAT_R32G32B32A32_SFLOAT);

    // Create Vulkan window container and put in layout
    vulkanWrapper =  QWidget::createWindowContainer(vulkanWindow);
    QGridLayout* layout = new QGridLayout();
    layout->addWidget(vulkanWrapper);
    this->setLayout(layout);

}

VulkanWindow* VulkanView::getVulkanWindow()
{
    return vulkanWindow;
}
