/*{
    "CATEGORIES": [
        "Geometry"
    ],
    "CREDIT": "VIDVOX",
    "INPUTS": [
        {
            "DEFAULT": 0.25,
            "NAME": "width",
			"LABEL": "Width",
            "TYPE": "float",
			"MIN": 0.01,
			"MAX": 1.0
        },
        {
            "DEFAULT": 0,
            "NAME": "offset",
			"LABEL": "Offset",
            "TYPE": "float",
			"MIN": 0.01,
			"MAX": 1.0
        },
        {
            "DEFAULT": 0,
            "NAME": "vertical",
			"LABEL": "Vertical",
            "TYPE": "bool"
        },
        {
            "DEFAULT": [
                1,
                1,
                1,
                1
            ],
            "NAME": "color1",
			"LABEL": "Color 1",
            "TYPE": "color"
        },
        {
            "DEFAULT": [
                0,
                0,
                0,
                1
            ],
            "NAME": "color2",
			"LABEL": "Color 2",
            "TYPE": "color"
        },
        {
            "DEFAULT": 0.5,
            "MAX": 1,
            "MIN": 0,
            "NAME": "splitPos",
			"LABEL": "Split Position",
            "TYPE": "float"
        }
    ],
    "ISFVSN": "2"
}
*/



void main() {
	//	determine if we are on an even or odd line
	//	math goes like..
	//	mod(((coord+offset) / width),2)
	
	
	vec4 out_color = color2;
	float coord = isf_FragNormCoord[0];

	if (vertical)	{
		coord = isf_FragNormCoord[1];
	}
	if (width == 0.0)	{
		out_color = color1;
	}
	else if(mod(((coord+offset) / width),2.0) < 2.0 * splitPos)	{
		out_color = color1;
	}
	
	gl_FragColor = out_color;
}