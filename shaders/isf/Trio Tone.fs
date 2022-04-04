/*{
	"CREDIT": "by VIDVOX",
	"ISFVSN": "2",
	"CATEGORIES": [
		"Color Effect"
	],
	"INPUTS": [
		{
			"NAME": "inputImage",
			"TYPE": "image"
		},
		{
			"NAME": "darkColor",
			"LABEL": "Dark Color",
			"TYPE": "color",
			"DEFAULT": [
				0.00,
				0.164,
				1.0,
				1.0
			]
		},
		{
			"NAME": "midColor",
			"LABEL": "Mid Color",
			"TYPE": "color",
			"DEFAULT": [
				0.00,
				1.00,
				0.00,
				1.0
			]
		},
		{
			"NAME": "brightColor",
			"LABEL": "Bright Color",
			"TYPE": "color",
			"DEFAULT": [
				1.0,
				0.00,
				0.00,
				1.0
			]
		}
	]
}*/


//	partly adapted from http://coding-experiments.blogspot.com/2010/10/thermal-vision-pixel-shader.html


void main ()	{
	vec4 pixcol = IMG_THIS_PIXEL(inputImage);
	vec4 color1 = vec4(0.0,0.0,0.0,1.0);
	vec4 color2 = darkColor;
	int ix = 0;
	
	const vec4 	lumacoeff = vec4(0.2126, 0.7152, 0.0722, 0.0);
	float		lum = dot(pixcol, lumacoeff);


	if (lum > 0.66)	{
		color1 = midColor;
		color2 = brightColor;
		ix = 2;
	}
	else if (lum > 0.33)	{
		color1 = darkColor;
		color2 = midColor;
		ix = 1;
	}
	
	vec4 thermal = mix(color1,color2,(lum-float(ix)*0.33)/0.33);
	
	gl_FragColor = vec4(thermal.rgb, pixcol.a);

}