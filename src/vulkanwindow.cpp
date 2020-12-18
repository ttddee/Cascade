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

#include "vulkanwindow.h"

#include <iostream>

#include <QMouseEvent>
#include <QLabel>

#include "renderer/vulkanrenderer.h"
#include "log.h"

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    CS_LOG_INFO("Creating renderer");

    // Make sure we keep our GPU resources when window loses focus
    this->setFlags(QVulkanWindow::PersistentResources);

    // If there is more than one GPU, pick the discrete one if possible
    auto props = availablePhysicalDevices();
    if (props.size() > 1)
    {
        bool discreteAvailable = false;

        for (int i = 0; i < props.size(); ++i)
        {
             if (props[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
             {
                 CS_LOG_INFO("Found discrete GPU.");
                 this->setPhysicalDeviceIndex(i);
                 discreteAvailable = true;

                 break;
             }
             else if (props[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
             {
                 CS_LOG_INFO("Found integrated GPU.");
             }
        }
        if(!discreteAvailable)
        {
            qWarning("No discrete GPU found.");
        }
    }
    else if (props.size() == 0)
    {
        emit noGPUFound();
    }

    renderer = new VulkanRenderer(this);

    return renderer;
}

VulkanRenderer* VulkanWindow::getRenderer()
{
    return renderer;
}

void VulkanWindow::handleZoomResetRequest()
{
    zoomFactor = 1.0;
    renderer->scale(1.0);
}

ViewerMode VulkanWindow::getViewerMode()
{
    return viewerMode;
}

void VulkanWindow::setViewerMode(const ViewerMode mode)
{
    viewerMode = mode;
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    isDragging = true;
    lastPos = e->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    isDragging = false;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (isDragging)
    {
        float dx = e->pos().x() - lastPos.x();
        float dy = e->pos().y() - lastPos.y();

        renderer->translate(dx, dy);
    }
    lastPos = e->pos();
}

void VulkanWindow::wheelEvent(QWheelEvent *e)
{
    if(e->angleDelta().y() > 0)
    {
        zoomFactor += zoomFactor * 0.1f;
    }
    else
    {
        zoomFactor -= zoomFactor * 0.1f;
    }

    //Limit the scale
    zoomFactor = zoomFactor > minZoom ? zoomFactor : minZoom;
    zoomFactor = zoomFactor < maxZoom ? zoomFactor : maxZoom;
    renderer->scale(zoomFactor);
}

VulkanWindow::~VulkanWindow()
{
    CS_LOG_INFO("Destroying VulkanWindow.");
}

