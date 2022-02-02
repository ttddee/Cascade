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

layout(push_constant) uniform pushConstants {
    layout(offset = 0) float bw;
    layout(offset = 4) float gamma;
    layout(offset = 8) float gain;
} pc;

float thresh = 0.00313066844250063;

void main()
{
    vec4 pixel = texture(tex, v_texcoord);

    if (pc.bw > 0.0)
    {
        float weights[3] = { 0.3086f, 0.6094f, 0.0820f };
        float avg = (pixel.r * weights[0] + pixel.g * weights[1] + pixel.b * weights[2]);
        pixel = vec4(avg, avg, avg, pixel.a);
    }
    if (pc.gamma != 1.0)
    {
        if (pc.gamma != 0.0)
        {
            pixel.r = pow(pixel.r, (1 / pc.gamma));
            pixel.g = pow(pixel.g, (1 / pc.gamma));
            pixel.b = pow(pixel.b, (1 / pc.gamma));
        }
        else
        {
            pixel = vec4(0.0, 0.0, 0.0, 0.0);
        }
    }
    if (pc.gain != 1.0)
    {
        pixel *= pc.gain;
    }

    // Linear to sRGB conversion
    for(int i = 0; i < 4; i++)
    {
        if(pixel[i] <= thresh)
        {
            pixel[i] *= 12.92;
        }
        else
        {
            pixel[i] = 1.055 * pow(pixel[i], 1 / 2.4) - 0.055;
        }
    }

    fragColor = pixel;
}
