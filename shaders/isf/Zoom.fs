/*{
    "CATEGORIES": [
        "Geometry Adjustment"
    ],
    "CREDIT": "by VIDVOX",
    "INPUTS": [
        {
            "NAME": "inputImage",
            "TYPE": "image"
        },
        {
            "DEFAULT": 1,
            "MAX": 10,
            "MIN": 0.01,
            "NAME": "level",
			"LABEL": "Level",
            "TYPE": "float"
        },
        {
            "DEFAULT": [
                0,
                0
            ],
            "MAX": [
                1,
                1
            ],
            "MIN": [
                0,
                0
            ],
            "NAME": "center",
			"LABEL": "Center",
            "TYPE": "point2D"
        }
    ],
    "ISFVSN": "2"
}
*/

void main() {
	vec2		loc;
	vec2		modifiedCenter;
	
	loc = isf_FragNormCoord;
	modifiedCenter = center;
	loc.x = (loc.x - modifiedCenter.x)*(1.0/level) + modifiedCenter.x;
	loc.y = (loc.y - modifiedCenter.y)*(1.0/level) + modifiedCenter.y;
	if ((loc.x < 0.0)||(loc.y < 0.0)||(loc.x > 1.0)||(loc.y > 1.0))	{
		gl_FragColor = vec4(0.0);
	}
	else	{
		gl_FragColor = IMG_NORM_PIXEL(inputImage,loc);
	}
}
