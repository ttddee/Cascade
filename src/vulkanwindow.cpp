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

#include "vulkanwindow.h"

#include <QMouseEvent>

#include "renderer/vulkanrenderer.h"
#include "log.h"

using Cascade::Renderer::VulkanRenderer;

namespace Cascade {

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    CS_LOG_INFO("Creating renderer");

    // Make sure we keep our GPU resources when window loses focus
    this->setFlags(QVulkanWindow::PersistentResources);

    // TODO: This does not work

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
            CS_LOG_WARNING("No discrete GPU found.");
        }
    }
    else if (props.size() == 0)
    {
        emit noGPUFound();
    }

    mRenderer = new VulkanRenderer(this);

    return mRenderer;
}

VulkanRenderer* VulkanWindow::getRenderer()
{
    return mRenderer;
}

void VulkanWindow::handleZoomResetRequest()
{
    mZoomFactor = 1.0;
    mRenderer->scale(1.0);
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    mIsDragging = true;
    mLastPos = e->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    mIsDragging = false;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (mIsDragging)
    {
        float dx = e->pos().x() - mLastPos.x();
        float dy = e->pos().y() - mLastPos.y();

        mRenderer->translate(dx, dy);
    }
    mLastPos = e->pos();
}

void VulkanWindow::wheelEvent(QWheelEvent *e)
{
    if(e->angleDelta().y() > 0)
    {
        mZoomFactor += mZoomFactor * 0.1f;
    }
    else
    {
        mZoomFactor -= mZoomFactor * 0.1f;
    }

    //Limit the scale
    mZoomFactor = mZoomFactor > mMinZoom ? mZoomFactor : mMinZoom;
    mZoomFactor = mZoomFactor < mMaxZoom ? mZoomFactor : mMaxZoom;
    mRenderer->scale(mZoomFactor);
}

VulkanWindow::~VulkanWindow()
{
    CS_LOG_INFO("Destroying VulkanWindow.");
}

} // namespace Cascade

