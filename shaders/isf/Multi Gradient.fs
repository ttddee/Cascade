/*
{
  "CATEGORIES" : [
    "Color"
  ],
  "ISFVSN" : "2",
  "INPUTS" : [
  	{
  	  "NAME" : "inputImage",
  	  "TYPE" : "image"
  	},
    {
      "NAME" : "frequency1",
	  "LABEL": "Frequency 1",
      "TYPE" : "float",
      "MAX" : 16,
      "DEFAULT" : 1.0,
      "MIN" : 0.5
    },
    {
      "NAME" : "phase1",
	  "LABEL": "Phase 1",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : 0
    },
    {
      "NAME" : "amplitude1",
	  "LABEL": "Amplitude 1",
      "TYPE" : "float",
      "MAX" : 2,
      "DEFAULT" : 1,
      "MIN" : -2
    },
    {
      "NAME" : "offset1",
	  "LABEL": "Offset 1",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : -1
    },
    {
      "NAME" : "angle1",
	  "LABEL": "Angle 1",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : 0
    },
    {
      "VALUES" : [
        0,
        1,
        2,
        3,
        4
      ],
      "NAME" : "curve1",
	  "LABEL": "Curve 1",
      "TYPE" : "long",
      "DEFAULT" : 0,
      "LABELS" : [
      	"Ramp",
        "Triangle",
        "Sine",
        "Exponential",
        "Look Up Table"
      ]
    },
    {
      "NAME" : "mixLevel1",
	  "LABEL": "Mix Level 1",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 1,
      "MIN" : -1
    },
    {
      "NAME" : "startColor1",
	  "LABEL": "Start Color 1",
      "TYPE" : "color",
      "DEFAULT" : [
        0,
        0,
        0,
        0
      ]
    },
    {
      "NAME" : "endColor1",
	  "LABEL": "End Color 1",
      "TYPE" : "color",
      "DEFAULT" : [
        1,
        0,
        0,
        1
      ]
    },
    {
      "NAME" : "frequency2",
	  "LABEL": "Frequency 2",
      "TYPE" : "float",
      "MAX" : 16,
      "DEFAULT" : 1.0,
      "MIN" : 0.5
    },
    {
      "NAME" : "phase2",
	  "LABEL": "Phase 2",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : 0
    },
    {
      "NAME" : "amplitude2",
	  "LABEL": "Amplitude 2",
      "TYPE" : "float",
      "MAX" : 2,
      "DEFAULT" : 1,
      "MIN" : -2
    },
    {
      "NAME" : "offset2",
	  "LABEL": "Offset 2",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : -1
    },
    {
      "NAME" : "angle2",
	  "LABEL": "Angle 2",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0.75,
      "MIN" : 0
    },
    {
      "VALUES" : [
        0,
        1,
        2,
        3,
        4
      ],
      "NAME" : "curve2",
	  "LABEL": "Curve 2",
      "TYPE" : "long",
      "DEFAULT" : 0,
      "LABELS" : [
      	"Ramp",
        "Triangle",
        "Sine",
        "Exponential",
        "Look Up Table"
      ]
    },
    {
      "NAME" : "mixLevel2",
	  "LABEL": "Mix Level 2",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 1,
      "MIN" : -1
    },
    {
      "NAME" : "startColor2",
	  "LABEL": "Start Color 2",
      "TYPE" : "color",
      "DEFAULT" : [
        0,
        0,
        0,
        0
      ]
    },
    {
      "NAME" : "endColor2",
	  "LABEL": "End Color 2",
      "TYPE" : "color",
      "DEFAULT" : [
        0,
        1,
        0,
        1
      ]
    },
    {
      "NAME" : "frequency3",
	  "LABEL": "Frequency 3",
      "TYPE" : "float",
      "MAX" : 16,
      "DEFAULT" : 2.0,
      "MIN" : 0.5
    },
    {
      "NAME" : "phase3",
	  "LABEL": "Phase 3",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : 0
    },
    {
      "NAME" : "amplitude3",
	  "LABEL": "Amplitude 3",
      "TYPE" : "float",
      "MAX" : 2,
      "DEFAULT" : 1,
      "MIN" : -2
    },
    {
      "NAME" : "offset3",
	  "LABEL": "Offset 3",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : -1
    },
    {
      "NAME" : "angle3",
	  "LABEL": "Angle 3",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 0,
      "MIN" : 0
    },
    {
      "VALUES" : [
        0,
        1,
        2,
        3,
        4
      ],
      "NAME" : "curve3",
	  "LABEL": "Curve 3",
      "TYPE" : "long",
      "DEFAULT" : 0,
      "LABELS" : [
      	"Ramp",
        "Triangle",
        "Sine",
        "Exponential",
        "Look Up Table"
      ]
    },
    {
      "NAME" : "mixLevel3",
	  "LABEL": "Mix Level 3",
      "TYPE" : "float",
      "MAX" : 1,
      "DEFAULT" : 1,
      "MIN" : -1
    },
    {
      "NAME" : "startColor3",
	  "LABEL": "Start Color 3",
      "TYPE" : "color",
      "DEFAULT" : [
        0,
        0,
        0,
        0
      ]
    },
    {
      "NAME" : "endColor3",
	  "LABEL": "End Color 3",
      "TYPE" : "color",
      "DEFAULT" : [
        0,
        0,
        1,
        1
      ]
    }
  ],
  "CREDIT" : "by Carter Rosenberg"
}
*/



const float pi = 3.14159265359;
const float e = 2.71828182846;


float doMath(int curve, float freq, float phase, float val)	{
	float	returnMe = phase + freq * val;

	if (curve == 0)	{
		returnMe = mod(returnMe,1.0);
	}
	else if (curve == 1)	{
		returnMe = mod(2.0 * returnMe,2.0);
		returnMe = (returnMe < 1.0) ? returnMe : 1.0 - (returnMe - floor(returnMe));
	}
	else if (curve == 2)	{
		returnMe = sin(returnMe * pi * 2.0 - pi / 2.0) * 0.5 + 0.5;
	}
	else if (curve == 3)	{
		returnMe = mod(2.0 * returnMe, 2.0);
		returnMe = (returnMe < 1.0) ? returnMe : 1.0 - (returnMe - floor(returnMe));
		returnMe = pow(returnMe, 2.0);
	}
	else if (curve == 4)	{
		vec2	loc = mod(returnMe+isf_FragNormCoord,1.0);
		vec4	tmp = IMG_NORM_PIXEL(inputImage,loc);
		returnMe = (tmp.r+tmp.g+tmp.b)*tmp.a/3.0;
	}
	return returnMe;	
}

//	note that this works on normalized points, but respects aspect ratio
vec2 rotatePoint(vec2 pt, float angle)	{
	vec2 returnMe = pt * RENDERSIZE;;

	float r = distance(RENDERSIZE/2.0, returnMe);
	float a = atan ((returnMe.y-RENDERSIZE.y/2.0),(returnMe.x-RENDERSIZE.x/2.0));

	returnMe.x = r * cos(a + 2.0 * pi * angle - pi) + 0.5;
	returnMe.y = r * sin(a + 2.0 * pi * angle - pi) + 0.5;
	
	returnMe = returnMe / RENDERSIZE + vec2(0.5);
	
	return returnMe;
}


void main() {
	vec4 returnMe = vec4(0.0);
	vec4 blendColor = vec4(0.0);
	float mixAmount = 0.0;
	vec2 loc = isf_FragNormCoord;

	loc = rotatePoint(isf_FragNormCoord,angle1);
	mixAmount = doMath(curve1,frequency1,phase1,1.0-loc.x);
	mixAmount = (amplitude1 >= 0.0) ? mixAmount * amplitude1 : (1.0 - mixAmount) * abs(amplitude1);
	mixAmount += offset1;
	blendColor = mix(startColor1,endColor1,mixAmount);
	returnMe.rgb += blendColor.rgb * mixLevel1;
	returnMe.a += abs(blendColor.a);
	
	loc = rotatePoint(isf_FragNormCoord,angle2);
	mixAmount = doMath(curve2,frequency2,phase2,1.0-loc.x);
	mixAmount = (amplitude2 >= 0.0) ? mixAmount * amplitude2 : (1.0 - mixAmount) * abs(amplitude2);
	mixAmount += offset2;
	blendColor = mix(startColor2,endColor2,mixAmount);
	returnMe.rgb += blendColor.rgb  * mixLevel2;
	returnMe.a += abs(blendColor.a);
	
	loc = rotatePoint(isf_FragNormCoord,angle3);
	mixAmount = doMath(curve3,frequency3,phase3,loc.x);
	mixAmount = (amplitude3 >= 0.0) ? mixAmount * amplitude3 : (1.0 - mixAmount) * abs(amplitude3);
	mixAmount += offset3;
	blendColor = mix(startColor3,endColor3,mixAmount);
	returnMe.rgb += blendColor.rgb  * mixLevel3;
	returnMe.a += abs(blendColor.a);
	
	gl_FragColor = returnMe;
}