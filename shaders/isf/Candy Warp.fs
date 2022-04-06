/*{
	"CREDIT": "by mojovideotech",
	"CATEGORIES" : [
    "Generator"
  ],
  "INPUTS" : [
	{
		"NAME" : 		"scale",
		"LABEL": "Scale",
		"TYPE" : 		"float",
		"DEFAULT" : 	84.0,
		"MIN" : 		10.0,
		"MAX" : 		100.0
	},
	{
		"NAME" : 		"cycle",
		"LABEL": "Cycle",
		"TYPE" : 		"float",
		"DEFAULT" : 	0.4,
		"MIN" : 		0.01,
		"MAX" : 		0.99
	},
	{
		"NAME" : 		"thickness",
		"LABEL": "Thickness",
		"TYPE" : 		"float",
		"DEFAULT" :  	0.1,
		"MIN" : 		-0.5,
		"MAX" : 		1.0
	},

	{
		"NAME" : 		"loops",
		"LABEL": "Loops",
		"TYPE" : 		"float",
		"DEFAULT" :	    61.0,
		"MIN" : 		10.0,
		"MAX" : 		100.0
	},
	{
		"NAME" : 		"warp",
		"LABEL": "Warp",
		"TYPE" : 		"float",
		"DEFAULT" : 	2.5,
		"MIN" : 		-5.0,
		"MAX" : 		5.0
	},
	{
      	"NAME" : 		"hue",
		"LABEL": "Hue",
      	"TYPE" : 		"float",
      	"DEFAULT" :		0.33,
      	"MIN" : 		-0.5,
      	"MAX" : 		0.5
   	},
   	{
      	"NAME" : 		"tint",
		"LABEL": "Tint",
      	"TYPE" : 		"float",
      	"DEFAULT" :		0.1,
      	"MIN" : 		-0.5,
      	"MAX" : 		0.5
    },
    {
		"NAME" : 		"rate",
		"LABEL": "Rate",
		"TYPE" : 		"float",
		"DEFAULT" : 	1.25,
		"MIN" : 		-3.0,
		"MAX" : 		3.0
	},
	    	{
   		"NAME" : 		"invert",
		"LABEL": "Invert",
     	"TYPE" : 		"bool",
     	"DEFAULT" : 	false
   	}
  ]
}
*/

////////////////////////////////////////////////////////////
// CandyWarp  by mojovideotech
//
// based on :  
// glslsandbox.com/e#38710.0
// Posted by Trisomie21
// modified by @hintz
//
// Creative Commons Attribution-NonCommercial-ShareAlike 3.0
////////////////////////////////////////////////////////////




void main(void)
{
	float s = RENDERSIZE.y / scale;
	float radius = RENDERSIZE.x / cycle;
	float gap = s * (1.0 - thickness);
	vec2 pos = gl_FragCoord.xy - RENDERSIZE.xy * 0.5;
	float d = length(pos);
	float T = TIME * rate;
	d += warp * (sin(pos.y * 0.25 / s + T) * sin(pos.x * 0.25 / s + T * 0.5)) * s * 5.0;
	float v = mod(d + radius / (loops * 2.0), radius / loops);
	v = abs(v - radius / (loops * 2.0));
	v = clamp(v - gap, 0.0, 1.0);
	d /= radius - T;
	vec3 m = fract((d - 1.0) * vec3(loops * hue, -loops, loops * tint) * 0.5);
	if (invert) 	gl_FragColor = vec4(m / v, 1.0);
	else gl_FragColor = vec4(m * v, 1.0);
}