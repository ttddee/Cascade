/*{
	"DESCRIPTION": "Your shader description",
	"CREDIT": "by MrBodean",
	"CATEGORIES": [
		"Utility"
	],
	"INPUTS": [
		{
			"NAME": "inputImage",
			"TYPE": "image"
		},
		{
			"NAME": "heightScale",
			"LABEL": "Height Scale",
			"TYPE": "float",
			"DEFAULT": 0.125,
			"MIN": -1.0,
			"MAX": 1.0
		}
      	]
}*/

// Ported from "Normal map calculation" by sergey_reznik: https://www.shadertoy.com/view/llS3WD

vec3 iResolution = vec3(RENDERSIZE, 1.);

//const float heightScale = 0.0125;

float sampleHeight(vec2 coord)
{
    return heightScale * 
        dot(IMG_NORM_PIXEL(inputImage, coord), vec4(1.0/3.0, 1.0/3.0, 1.0/3.0, 0.0));
} 
   
void main(void)
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec2 du = vec2(1.0 / 1024.0, 0.0);
    vec2 dv = vec2(0.0, 1.0 / 1024.0);
    
    float h0 = sampleHeight(uv);
    float hpx = sampleHeight(uv + du);
    float hmx = sampleHeight(uv - du);
    float hpy = sampleHeight(uv + dv);
    float hmy = sampleHeight(uv - dv);
    
    float dHdU = (hmx - hpx) / (2.0 * du.x);
    float dHdV = (hmy - hpy) / (2.0 * dv.y);
    
    vec3 normal = normalize(vec3(dHdU, dHdV, 1.0));
   
	gl_FragColor = vec4(0.5 + 0.5 * normal, 1.0);
}
