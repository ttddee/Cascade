/*
 *  Cascade Image Editor
 *
 *  Copyright (C) 2022 Till Dechent and contributors
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#version 430

layout(location = 0) in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform sampler2D upstream;

layout(push_constant) uniform pushConstants {
    layout(offset = 0) float split;
	layout(offset = 4) float splitPercent;
    layout(offset = 8) float bw;
    layout(offset = 12) float gamma;
    layout(offset = 16) float gain;
} pc;

float thresh = 0.00313066844250063;

void main()
{
	ivec2 imageSize = textureSize(tex,0);

    float pixel = texture(tex, v_texcoord).a;
	
	if (pc.split > 0.5)
	{
		int xPos = int(imageSize.x * v_texcoord.x);
		if (v_texcoord.x < pc.splitPercent)
		{
			pixel = texture(upstream, v_texcoord).a;
		}
		if (int(imageSize.x * pc.splitPercent) == xPos)
		{
			pixel = 0.0;
		}
	}

    if (pc.gamma != 1.0)
    {
        if (pc.gamma != 0.0)
        {
            pixel = pow(pixel, (1 / pc.gamma));
        }
        else
        {
            pixel = 0.0;
        }
    }
    if (pc.gain != 1.0)
    {
        pixel *= pc.gain;
    }

    // Linear to sRGB conversion
    if(pixel <= thresh)
    {
        pixel *= 12.92;
    }
    else
    {
        pixel = 1.055 * pow(pixel, 1 / 2.4) - 0.055;
    }

    fragColor = vec4(pixel, pixel, pixel, pixel);
}
