/*
Software License :

Copyright (c) 2007, The Open Effects Association Ltd. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name The Open Effects Association Ltd, nor the names of its 
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <ctime>

// ofx
#include "ofxCore.h"
#include "ofxImageEffect.h"
#include "ofxPixels.h"

// ofx host
#include "ofxhBinary.h"
#include "ofxhPropertySuite.h"
#include "ofxhClip.h"
#include "ofxhParam.h"
#include "ofxhMemory.h"
#include "ofxhImageEffect.h"
#include "ofxhPluginAPICache.h"
#include "ofxhPluginCache.h"
#include "ofxhHost.h"
#include "ofxhImageEffectAPI.h"

// my host
#include "CsOfxHostDescriptor.h"
#include "CsOfxEffectInstance.h"
#include "CsOfxClipInstance.h"

// We are hard coding everything in our example, in a real host you
// need to enquire things from your host.
namespace CsOfxHost {
//const double    kPalPixelAspect = double(768)/double(720);
//const int       kPalSizeXPixels = 720;
//const int       kPalSizeYPixels = 576;
//const OfxRectI  kPalRegionPixels = {0, 0, kPalSizeXPixels, kPalSizeYPixels};
//const OfxRectD  kPalRegionCanon = {0,0, kPalSizeXPixels * kPalPixelAspect ,kPalSizeYPixels};

// 5x3 bitmaps for digits 0..9 and period
const char digits[11][5][3] = {
  { {0,1,0},
    {1,0,1},
    {1,0,1},
    {1,0,1},
    {0,1,0} },
  { {0,1,0},
    {1,1,0},
    {0,1,0},
    {0,1,0},
    {0,1,0} },
  { {0,1,0},
    {1,0,1},
    {0,0,1},
    {0,1,0},
    {1,1,1} },
  { {1,1,0},
    {0,0,1},
    {0,1,1},
    {0,0,1},
    {1,1,0} },
  { {0,1,0},
    {1,0,0},
    {1,0,1},
    {1,1,1},
    {0,0,1} },
  { {1,1,1},
    {1,0,0},
    {1,1,0},
    {0,0,1},
    {1,1,0} },
  { {0,1,1},
    {1,0,0},
    {1,1,0},
    {1,0,1},
    {0,1,0} },
  { {1,1,1},
    {0,0,1},
    {0,1,0},
    {0,1,1},
    {0,1,0} },
  { {0,1,0},
    {1,0,1},
    {0,1,0},
    {1,0,1},
    {0,1,0} },
  { {0,1,0},
    {1,0,1},
    {0,1,1},
    {0,0,1},
    {1,1,0} },
  { {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,1,0} }
};

// draw digit d at x,y int the width*height image pointed to by data
static void drawDigit(OfxRGBAColourB* data, int width, int height, int d, int x, int y , int scale, OfxRGBAColourB color) {
  assert(0 <= x && x+3*scale < width);
  assert(0 <= y && y+5*scale < height);

  for (int j = 0; j < 5; ++j) {
    for (int i = 0; i < 3; ++i) {
      if (digits[d][j][i]) {
        for (int jj = 0; jj < scale; ++jj) {
          for (int ii = 0; ii < scale; ++ii) {
            int x1 = x + i*scale + ii;
            int y1 = y + j*scale + jj;
            data[x1+y1*width] = color;
          }
        }
      }
    }
  }
}

/// images are always SD PAL progressive full res images for the purpose of this example only
CsOfxImage::CsOfxImage(
        CsOfxClipInstance &clip,
        OfxTime time,
        const OfxPointI s,
        float* pInput,
        int view)
  : OFX::Host::ImageEffect::Image(clip) /// this ctor will set basic props on the image
  , _data(NULL)
{
    size = s;

  // make some memory
  _data = new OfxRGBAColourB[size.x * size.y] ;

  int fillValue = (int)(floor(255.0 * (time/OFXCLIPLENGTH))) & 0xff;
  OfxRGBAColourB color;
  color.r = color.g = color.b = fillValue;
  color.a = 215;

  std::fill(_data, _data + size.x * size.y, color);
  // draw the time and the view number in reverse color
  const int scale = 5;
  const int charwidth = 4*scale;
  color.r = color.g = color.b = 255-fillValue;
//  int xx = 50;
//  int yy = 50;
//  int d;
//  d = (int(time)/10)%10;
//  drawDigit(_data, size.x, size.y, d, xx, yy, scale, color);
//  xx += charwidth;
//  d = int(time)%10;
//  drawDigit(_data, size.x, size.y, d, xx, yy, scale, color);
//  xx += charwidth;
//  d = 10;
//  drawDigit(_data, size.x, size.y, d, xx, yy, scale, color);
//  xx += charwidth;
//  d = int(time*10)%10;
//  drawDigit(_data, size.x, size.y, d, xx, yy, scale, color);
//  xx = 50;
//  yy += 8*scale;
//  d = int(view)%10;
//  drawDigit(_data, size.x, size.y, d, xx, yy, scale, color);

  // render scale x and y of 1.0
  setDoubleProperty(kOfxImageEffectPropRenderScale, 1.0, 0);
  setDoubleProperty(kOfxImageEffectPropRenderScale, 1.0, 1);

  // data ptr
  setPointerProperty(kOfxImagePropData,_data);

  // bounds and rod
  setIntProperty(kOfxImagePropBounds, 0, 0);
  setIntProperty(kOfxImagePropBounds, 0, 1);
  setIntProperty(kOfxImagePropBounds, size.x, 2);
  setIntProperty(kOfxImagePropBounds, size.y, 3);

  setIntProperty(kOfxImagePropRegionOfDefinition, 0, 0);
  setIntProperty(kOfxImagePropRegionOfDefinition, 0, 1);
  setIntProperty(kOfxImagePropRegionOfDefinition, size.x, 2);
  setIntProperty(kOfxImagePropRegionOfDefinition, size.y, 3);

  // row bytes
  setIntProperty(kOfxImagePropRowBytes, size.x * sizeof(OfxRGBAColourB));
}

OfxPointI CsOfxImage::getSize()
{
    return size;
}

OfxRGBAColourB* CsOfxImage::pixel(int x, int y) const
{
  OfxRectI bounds = getBounds();
  if ((x >= bounds.x1) && ( x< bounds.x2) && ( y >= bounds.y1) && ( y < bounds.y2) )
  {
    int rowBytes = getIntProperty(kOfxImagePropRowBytes);
    int offset = (y - bounds.y1) * rowBytes + (x - bounds.x1) * sizeof(OfxRGBAColourB);
    return reinterpret_cast<OfxRGBAColourB*>(&(reinterpret_cast<char*>(_data)[offset]));
  }
  return 0;
}

CsOfxImage::~CsOfxImage()
{
    std::cout << "Destroying CsOfxImage" << std::endl;
    delete _data;
}

CsOfxClipInstance::CsOfxClipInstance(
        CsOfxEffectInstance* effect,
        OFX::Host::ImageEffect::ClipDescriptor *desc)
  : OFX::Host::ImageEffect::ClipInstance(effect, *desc)
  , _effect(effect)
  , _name(desc->getName())
  , _outputImage(NULL)
{
}

CsOfxClipInstance::~CsOfxClipInstance()
{
  if(_outputImage)
    _outputImage->releaseReference();
}

void CsOfxClipInstance::setSourceImage(float *pImage, const OfxPointI imgSize)
{
    pSourceImage = pImage;
    sourceImageSize = imgSize;
}

/// Get the Raw Unmapped Pixel Depth from the host. We are always 8 bits in our example
const std::string &CsOfxClipInstance::getUnmappedBitDepth() const
{
  static const std::string v(kOfxBitDepthByte);
  return v;
}

/// Get the Raw Unmapped Components from the host. In our example we are always RGBA
const std::string &CsOfxClipInstance::getUnmappedComponents() const
{
  static const std::string v(kOfxImageComponentRGBA);
  return v;
}

// PreMultiplication -
//
//  kOfxImageOpaque - the image is opaque and so has no premultiplication state
//  kOfxImagePreMultiplied - the image is premultiplied by it's alpha
//  kOfxImageUnPreMultiplied - the image is unpremultiplied
const std::string &CsOfxClipInstance::getPremult() const
{
  static const std::string v(kOfxImageUnPreMultiplied);
  return v;
}

// Pixel Aspect Ratio -
//
//  The pixel aspect ratio of a clip or image.
double CsOfxClipInstance::getAspectRatio() const
{
  return 1.0;
}

// Frame Rate -
double CsOfxClipInstance::getFrameRate() const
{
  /// our clip is pretending to be progressive PAL SD, so return 25
  return 25.0;
}

// Frame Range (startFrame, endFrame) -
//
//  The frame range over which a clip has images.
void CsOfxClipInstance::getFrameRange(double &startFrame, double &endFrame) const
{
  // pretend we have a second's worth of PAL SD
  startFrame = 0;
  endFrame = 1;
}

/// Field Order - Which spatial field occurs temporally first in a frame.
/// \returns
///  - kOfxImageFieldNone - the clip material is unfielded
///  - kOfxImageFieldLower - the clip material is fielded, with image rows 0,2,4.... occuring first in a frame
///  - kOfxImageFieldUpper - the clip material is fielded, with image rows line 1,3,5.... occuring first in a frame
const std::string &CsOfxClipInstance::getFieldOrder() const
{
  /// our clip is pretending to be progressive PAL SD, so return kOfxImageFieldNone
  static const std::string v(kOfxImageFieldNone);
  return v;
}

// Connected -
//
//  Says whether the clip is actually connected at the moment.
bool CsOfxClipInstance::getConnected() const
{
  return true;
}

// Unmapped Frame Rate -
//
//  The unmaped frame range over which an output clip has images.
double CsOfxClipInstance::getUnmappedFrameRate() const
{
  /// our clip is pretending to be progressive PAL SD, so return 25
  return 25;
}

// Unmapped Frame Range -
//
//  The unmaped frame range over which an output clip has images.
// this is applicable only to hosts and plugins that allow a plugin to change frame rates
void CsOfxClipInstance::getUnmappedFrameRange(double &unmappedStartFrame, double &unmappedEndFrame) const
{
  unmappedStartFrame = 0;
  unmappedEndFrame = 1;
}

// Continuous Samples -
//
//  0 if the images can only be sampled at discreet times (eg: the clip is a sequence of frames),
//  1 if the images can only be sampled continuously (eg: the clip is infact an animating roto spline and can be rendered anywhen).
bool CsOfxClipInstance::getContinuousSamples() const
{
  return false;
}


/// override this to return the rod on the clip canonical coords!
OfxRectD CsOfxClipInstance::getRegionOfDefinition(OfxTime time) const
{
  OfxRectD v;
  v.x1 = v.y1 = 0;
  v.x2 = _outputImage->getSize().x;
  v.y2 = _outputImage->getSize().y;
  return v;
}

/// override this to fill in the image at the given time.
/// The bounds of the image on the image plane should be
/// 'appropriate', typically the value returned in getRegionsOfInterest
/// on the effect instance. Outside a render call, the optionalBounds should
/// be 'appropriate' for the.
/// If bounds is not null, fetch the indicated section of the canonical image plane.
OFX::Host::ImageEffect::Image* CsOfxClipInstance::getImage(
        OfxTime time,
        const OfxRectD *optionalBounds)
{
  if(_name == "Output") {
    if(!_outputImage) {
      // make a new ref counted image
      _outputImage = new CsOfxImage(*this, 0, sourceImageSize);
    }

    // add another reference to the member image for this fetch
    // as we have a ref count of 1 due to construction, this will
    // cause the output image never to delete by the plugin
    // when it releases the image
    _outputImage->addReference();

    // return it
    return _outputImage;
  }
  else {
    // Fetch on demand for the input clip.
    // It does get deleted after the plugin is done with it as we
    // have not incremented the auto ref
    //
    // You should do somewhat more sophisticated image management
    // than this.
      // THIS IS WHAT WE NEED
    CsOfxImage *image = new CsOfxImage(*this, 0, sourceImageSize);

    return image;
  }
}
} // MyHost
