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
#include "CsOfxParamInstance.h"

namespace CsOfxHost {

  //
  // MyIntegerInstance
  //

  MyIntegerInstance::MyIntegerInstance(CsOfxEffectInstance* effect,
                                       const std::string& name, 
                                       OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::IntegerInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyIntegerInstance::get(int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyIntegerInstance::get(OfxTime time, int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyIntegerInstance::set(int)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyIntegerInstance::set(OfxTime time, int) {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyDoubleInstance
  //

  MyDoubleInstance::MyDoubleInstance(CsOfxEffectInstance* effect,
                                     const std::string& name, 
                                     OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::DoubleInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyDoubleInstance::get(double& d)
  {
    // values for the Basic OFX plugin to work
    d = 2.0;
    return kOfxStatOK;
  }

  OfxStatus MyDoubleInstance::get(OfxTime time, double& d)
  {
    // values for the Basic OFX plugin to work
    d = 2.0;
    return kOfxStatOK;
  }

  OfxStatus MyDoubleInstance::set(double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDoubleInstance::set(OfxTime time, double) 
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDoubleInstance::derive(OfxTime time, double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDoubleInstance::integrate(OfxTime time1, OfxTime time2, double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyBooleanInstance
  //

  MyBooleanInstance::MyBooleanInstance(CsOfxEffectInstance* effect,
                                       const std::string& name, 
                                       OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::BooleanInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyBooleanInstance::get(bool& b)
  {
    b = true;
    return kOfxStatOK;
  }

  OfxStatus MyBooleanInstance::get(OfxTime time, bool& b)
  {
    b = true;
    return kOfxStatOK;
  }

  OfxStatus MyBooleanInstance::set(bool)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyBooleanInstance::set(OfxTime time, bool) {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyChoiceInteger
  //

  MyChoiceInstance::MyChoiceInstance(CsOfxEffectInstance* effect,
                                     const std::string& name, 
                                     OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::ChoiceInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyChoiceInstance::get(int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyChoiceInstance::get(OfxTime time, int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyChoiceInstance::set(int)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyChoiceInstance::set(OfxTime time, int) 
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyRGBAInstance
  //

  MyRGBAInstance::MyRGBAInstance(CsOfxEffectInstance* effect,
                                 const std::string& name, 
                                 OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::RGBAInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyRGBAInstance::get(double&,double&,double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBAInstance::get(OfxTime time, double&,double&,double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBAInstance::set(double,double,double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBAInstance::set(OfxTime time, double,double,double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyRGBInstance
  //

  MyRGBInstance::MyRGBInstance(CsOfxEffectInstance* effect,
                               const std::string& name, 
                               OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::RGBInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyRGBInstance::get(double&,double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBInstance::get(OfxTime time, double&,double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBInstance::set(double,double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyRGBInstance::set(OfxTime time, double,double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyDouble2DInstance
  //

  MyDouble2DInstance::MyDouble2DInstance(CsOfxEffectInstance* effect,
                                         const std::string& name, 
                                         OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::Double2DInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyDouble2DInstance::get(double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDouble2DInstance::get(OfxTime time,double&,double&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDouble2DInstance::set(double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyDouble2DInstance::set(OfxTime time,double,double)
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyInteger2DInstance
  //

  MyInteger2DInstance::MyInteger2DInstance(CsOfxEffectInstance* effect,
                                           const std::string& name, 
                                           OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::Integer2DInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

  OfxStatus MyInteger2DInstance::get(int&,int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyInteger2DInstance::get(OfxTime time,int&,int&)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyInteger2DInstance::set(int,int)
  {
    return kOfxStatErrMissingHostFeature;
  }

  OfxStatus MyInteger2DInstance::set(OfxTime time,int,int)
  {
    return kOfxStatErrMissingHostFeature;
  }

  //
  // MyInteger2DInstance
  //

  MyPushbuttonInstance::MyPushbuttonInstance(CsOfxEffectInstance* effect,
                                             const std::string& name, 
                                             OFX::Host::Param::Descriptor& descriptor)
    : OFX::Host::Param::PushbuttonInstance(descriptor), _effect(effect), _descriptor(descriptor)
  {
  }

}
