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

CsImage::~CsImage()
{
    // TODO: destroy view and image here
}
