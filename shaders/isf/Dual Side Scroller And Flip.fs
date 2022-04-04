/*{
	"CREDIT": "Inspired by Side Scroller and Flip by BrianChasalow",
	"ISFVSN": "2",
	 "CATEGORIES": [
	 	"Geometry Adjustment"
	 ],
	 "INPUTS": [
		 {
			 "NAME": "inputImage",
			 "TYPE": "image"
		 },
		 {
			 "NAME": "slidetop",
			 "LABEL": "Slide Top",
			 "TYPE": "float",
			 "MIN": 0.0,
			 "MAX": 2.0,
			 "DEFAULT": 0.0
		 },
		 {
			 "NAME": "shifttop",
			 "LABEL": "Shift Top",
			 "TYPE": "float",
			 "MIN": 0.0,
			 "MAX": 2.0,
			 "DEFAULT": 0.0
		 },
		 {
			 "NAME": "mirrorHorizontaltop",
			 "LABEL": "Mirror Horizontal Top",
			 "TYPE": "bool",
			 "MIN": false,
			 "MAX": true,
			 "DEFAULT": true
		 },
		 {
			 "NAME": "mirrorVerticaltop",
			 "LABEL": "Mirror Vertical Top",
			 "TYPE": "bool",
			 "MIN": false,
			 "MAX": true,
			 "DEFAULT": true
		 },
		 {
			 "NAME": "slidebot",
			 "LABEL": "Slide Bottom",
			 "TYPE": "float",
			 "MIN": 0.0,
			 "MAX": 2.0,
			 "DEFAULT": 0.0
		 },
		 {
			 "NAME": "shiftbot",
			 "LABEL": "Shift Bottom",
			 "TYPE": "float",
			 "MIN": 0.0,
			 "MAX": 2.0,
			 "DEFAULT": 0.0
		 },
		 {
			 "NAME": "mirrorHorizontalbot",
			 "LABEL": "Mirror Horizontal Bottom",
			 "TYPE": "bool",
			 "MIN": false,
			 "MAX": true,
			 "DEFAULT": true
		 },
		 {
			 "NAME": "mirrorVerticalbot",
			 "LABEL": "Mirror Vertical Bottom",
			 "TYPE": "bool",
			 "MIN": false,
			 "MAX": true,
			 "DEFAULT": true
		 }
 
	 ]
 }*/

void main(void)
{
	vec2 pt = isf_FragNormCoord;
	float slide = (isf_FragNormCoord.y > 0.5) ? slidetop : slidebot;
	float shift = (isf_FragNormCoord.x < 0.5) ? shifttop : shiftbot;

	bool mirrorHorizontal = (isf_FragNormCoord.y > 0.5) ? mirrorHorizontaltop : mirrorHorizontalbot;
	bool mirrorVertical = (isf_FragNormCoord.x < 0.5) ? mirrorVerticaltop : mirrorVerticalbot;
	pt.x += slide;
	pt.y += shift;
	vec2 moddedRetard = mod(pt,1.0);
	
	if(mirrorHorizontal && pt.x >= 1.0 && pt.x <= 2.0)
		moddedRetard = vec2(1.0-moddedRetard.x, moddedRetard.y);
	if(mirrorVertical && pt.y >= 1.0 && pt.y <= 2.0)
		moddedRetard = vec2(moddedRetard.x, 1.0-moddedRetard.y);
	
	vec4 pixel = IMG_NORM_PIXEL(inputImage, moddedRetard);
	gl_FragColor = pixel;
}