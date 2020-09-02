#include "vulkanwindow.h"

#include <iostream>

#include <QMouseEvent>

#include "vulkanrenderer.h"

QVulkanWindowRenderer *VulkanWindow::createRenderer()
{
    qDebug("Creating renderer");

    renderer = new VulkanRenderer(this);

    return renderer;
}

VulkanRenderer* VulkanWindow::getRenderer()
{
    return renderer;
}

bool VulkanWindow::getShowOriginal()
{
    return showOriginal;
}

void VulkanWindow::setShowOriginal(bool b)
{
    showOriginal = b;
}

void VulkanWindow::mousePressEvent(QMouseEvent *e)
{
    pressed = true;
    lastPos = e->pos();
}

void VulkanWindow::mouseReleaseEvent(QMouseEvent *)
{
    pressed = false;
}

void VulkanWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (!pressed)
        return;

    float dx        = e->pos().x() - lastPos.x();
    float dy        = e->pos().y() - lastPos.y();
    float magnitude = std::max(abs(dx),abs(dy));

    if (  magnitude > 0.0f )
    {
        renderer->translate(dx, dy);
    }

    lastPos = e->pos();
}

void VulkanWindow::wheelEvent(QWheelEvent *e)
{
    e->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;


    //Limit the scale,
    scale = scale > 0.1f ? scale : 0.1f;
    scale = scale < 5.0f ? scale : 5.0f;
    renderer->scale(scale);
}

