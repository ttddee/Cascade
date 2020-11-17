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
#ifndef HOST_DEMO_PARAM_INSTANCE_H
#define HOST_DEMO_PARAM_INSTANCE_H

#include "ofxhImageEffect.h"
#include "CsOfxEffectInstance.h"

namespace CsOfxHost {

  class MyPushbuttonInstance : public OFX::Host::Param::PushbuttonInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyPushbuttonInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
  };

  class MyIntegerInstance : public OFX::Host::Param::IntegerInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyIntegerInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(int&);
    OfxStatus get(OfxTime time, int&);
    OfxStatus set(int);
    OfxStatus set(OfxTime time, int);
  };

  class MyDoubleInstance : public OFX::Host::Param::DoubleInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyDoubleInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(double&);
    OfxStatus get(OfxTime time, double&);
    OfxStatus set(double);
    OfxStatus set(OfxTime time, double);
    OfxStatus derive(OfxTime time, double&);
    OfxStatus integrate(OfxTime time1, OfxTime time2, double&);
  };

  class MyBooleanInstance : public OFX::Host::Param::BooleanInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyBooleanInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(bool&);
    OfxStatus get(OfxTime time, bool&);
    OfxStatus set(bool);
    OfxStatus set(OfxTime time, bool);
  };

  class MyChoiceInstance : public OFX::Host::Param::ChoiceInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyChoiceInstance(CsOfxEffectInstance* effect,  const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(int&);
    OfxStatus get(OfxTime time, int&);
    OfxStatus set(int);
    OfxStatus set(OfxTime time, int);
  };

  class MyRGBAInstance : public OFX::Host::Param::RGBAInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyRGBAInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(double&,double&,double&,double&);
    OfxStatus get(OfxTime time, double&,double&,double&,double&);
    OfxStatus set(double,double,double,double);
    OfxStatus set(OfxTime time, double,double,double,double);
  };


  class MyRGBInstance : public OFX::Host::Param::RGBInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyRGBInstance(CsOfxEffectInstance* effect,  const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(double&,double&,double&);
    OfxStatus get(OfxTime time, double&,double&,double&);
    OfxStatus set(double,double,double);
    OfxStatus set(OfxTime time, double,double,double);
  };

  class MyDouble2DInstance : public OFX::Host::Param::Double2DInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyDouble2DInstance(CsOfxEffectInstance* effect, const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(double&,double&);
    OfxStatus get(OfxTime time,double&,double&);
    OfxStatus set(double,double);
    OfxStatus set(OfxTime time,double,double);
  };

  class MyInteger2DInstance : public OFX::Host::Param::Integer2DInstance {
  protected:
    CsOfxEffectInstance*   _effect;
    OFX::Host::Param::Descriptor& _descriptor;
  public:
    MyInteger2DInstance(CsOfxEffectInstance* effect,  const std::string& name, OFX::Host::Param::Descriptor& descriptor);
    OfxStatus get(int&,int&);
    OfxStatus get(OfxTime time,int&,int&);
    OfxStatus set(int,int);
    OfxStatus set(OfxTime time,int,int);
  };


}

#endif // HOST_DEMO_PARAM_INSTANCE_H
