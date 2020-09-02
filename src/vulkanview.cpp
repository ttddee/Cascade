#include "vulkanview.h"

#include <QApplication>
#include <QHBoxLayout>

VulkanView::VulkanView(QWidget *parent) : QWidget(parent)
{
    qDebug("Creating Vulkan instance");

    // Set up validation layers
#ifdef QT_DEBUG
    instance.setLayers(QByteArrayList()
                       << "VK_LAYER_LUNARG_parameter_validation"
                       << "VK_LAYER_LUNARG_object_tracker"
                       << "VK_LAYER_LUNARG_core_validation"
                       << "VK_LAYER_LUNARG_image"
                       << "VK_LAYER_LUNARG_swapchain");
#endif

    if (!instance.create())
        qFatal("Failed to create Vulkan instance: %d", instance.errorCode());


    // Create a VulkanWindow
    vulkanWindow = new VulkanWindow();
    vulkanWindow->setVulkanInstance(&instance);

    vulkanWindow->setPreferredColorFormats(QVector<VkFormat>() << VK_FORMAT_B8G8R8A8_SRGB);

    // Create Vulkan window container and put in layout
    vulkanWrapper =  QWidget::createWindowContainer(vulkanWindow);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(vulkanWrapper);
    this->setLayout(layout);

}

VulkanWindow* VulkanView::getVulkanWindow()
{
    return vulkanWindow;
}
