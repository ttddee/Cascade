#ifndef CSIMAGE_H
#define CSIMAGE_H

#include <vulkan/vulkan.h>

class CsImage
{
public:
    CsImage(const int w = 100, const int h = 100);

    VkImage& getImage();
    VkImageView& getImageView();

    ~CsImage();

private:
    VkImage image = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;

    const int width;
    const int height;
};

#endif // CSIMAGE_H
