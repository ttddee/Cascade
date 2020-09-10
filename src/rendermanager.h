#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <QObject>

class VulkanRenderer;

class RenderManager : public QObject
{
    Q_OBJECT

public:
    static RenderManager& getInstance();
    RenderManager(RenderManager const&) = delete;
    void operator=(RenderManager const&) = delete;
    void setUp(VulkanRenderer* r);

private:
    RenderManager() {}

    VulkanRenderer* renderer;

signals:

public slots:

};

#endif // RENDERMANAGER_H
