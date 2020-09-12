#include "csimage.h"

CsImage::CsImage(const int w, const int h)
    : width(w),
      height(h)
{

}

VkImage& CsImage::getImage()
{
    return image;
}

VkImageView& CsImage::getImageView()
{
    return view;
}

int CsImage::getWidth() const
{
    return width;
}

int CsImage::getHeight() const
{
    return height;
}

CsImage::~CsImage()
{
    // TODO: destroy view and image here
}
