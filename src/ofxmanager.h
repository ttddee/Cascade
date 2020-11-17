#ifndef OFXMANAGER_H
#define OFXMANAGER_H

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <sstream> // stringstream

// ofx
#include "ofxCore.h"
#include "ofxImageEffect.h"
#include "ofxPixels.h"

// ofx host
#include "ofx/ofxhBinary.h"
#include "ofx/ofxhPropertySuite.h"
#include "ofx/ofxhClip.h"
#include "ofx/ofxhParam.h"
#include "ofx/ofxhMemory.h"
#include "ofx/ofxhImageEffect.h"
#include "ofx/ofxhPluginAPICache.h"
#include "ofx/ofxhPluginCache.h"
#include "ofx/ofxhHost.h"
#include "ofx/ofxhImageEffectAPI.h"

// my host
#include "ofx/CsOfxHostDescriptor.h"
#include "ofx/CsOfxEffectInstance.h"
#include "ofx/CsOfxClipInstance.h"

class OfxManager
{
public:
    OfxManager();

    std::shared_ptr<CsOfxHost::CsOfxImage> renderOFX(
            const OfxPointI renderSize,
            float* pInput);

private:
    void exportToPPM(const std::string& fname, CsOfxHost::CsOfxImage* im);

    CsOfxHost::Host ofxHost;
    OFX::Host::ImageEffect::PluginCache imageEffectPluginCache;
    OFX::Host::ImageEffect::ImageEffectPlugin* plugin;
    OFX::Host::auto_ptr<OFX::Host::ImageEffect::Instance> instance;
    CsOfxHost::CsOfxClipInstance* outputClip;
    std::shared_ptr<CsOfxHost::CsOfxImage> outputImage;
};

#endif // OFXMANAGER_H
