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

VulkanView::VulkanView(QWidget *parent) : QWidget(parent)
{
    qDebug("Creating Vulkan instance");

    // Set up validation layers
#ifdef QT_DEBUG
    instance.setLayers(QByteArrayList()
                       << "VK_LAYER_GOOGLE_threading"
                       << "VK_LAYER_LUNARG_parameter_validation"
                       << "VK_LAYER_LUNARG_object_tracker"
                       << "VK_LAYER_LUNARG_core_validation"
                       << "VK_LAYER_LUNARG_image"
                       << "VK_LAYER_LUNARG_swapchain"
                       << "VK_LAYER_GOOGLE_unique_objects");
#endif

    if (!instance.create())
        qFatal("Failed to create Vulkan instance: %d", instance.errorCode());


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
