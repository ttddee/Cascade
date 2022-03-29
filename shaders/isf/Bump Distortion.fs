/*{
  "CREDIT": "by carter rosenberg",
  "CATEGORIES": [
    "Distortion Effect"
  ],
  "INPUTS": [
    {
      "NAME": "inputImage",
      "TYPE": "image"
    },
    {
      "NAME": "level",
      "TYPE": "float",
	  "LABEL": "Level",
      "MIN": -1,
      "MAX": 1,
      "DEFAULT": 0.5
    },
    {
      "NAME": "radius",
      "TYPE": "float",
	  "LABEL": "Radius",
      "MIN": 0,
      "MAX": 1,
      "DEFAULT": 0.25
    },
    {
      "NAME": "center",
      "TYPE": "point2D",
	  "LABEL": "Center",
      "DEFAULT": [
        0.5,
        0.5
      ],
      "MIN": [
        0.0,
        0.0
      ],
      "MAX": [
        1.0,
        1.0
      ]
    }
  ]
}*/

const float pi = 3.14159265359;



void main() 
{
	vec2 uv = vec2(isf_FragNormCoord[0],isf_FragNormCoord[1]);
	vec2 texSize = RENDERSIZE;
	vec2 tc = uv * texSize;
	vec2 modifiedCenter = center * texSize;
	float r = distance(modifiedCenter, tc);
	float a = atan ((tc.y-modifiedCenter.y),(tc.x-modifiedCenter.x));
	float radius_sized = radius * length(RENDERSIZE);
	
	tc -= modifiedCenter;

	if (r < radius_sized) 	{
		float percent = 1.0-(radius_sized - r) / radius_sized;
		if (level>=0.0)	{
			percent = percent * percent;
			tc.x = r*pow(percent,level) * cos(a);
			tc.y = r*pow(percent,level) * sin(a);
		}
		else	{
			float adjustedLevel = level/2.0;
			tc.x = r*pow(percent,adjustedLevel) * cos(a);
			tc.y = r*pow(percent,adjustedLevel) * sin(a);		
		}
	}
	tc += modifiedCenter;
	vec2 loc = tc / texSize;

	if ((loc.x < 0.0)||(loc.y < 0.0)||(loc.x > 1.0)||(loc.y > 1.0))	{
		gl_FragColor = vec4(0.0);
	}
	else	{
		gl_FragColor = IMG_NORM_PIXEL(inputImage, loc);
	}
}