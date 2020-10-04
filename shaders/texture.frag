#version 440

layout(location = 0) in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

float thresh = 0.00313066844250063;

void main()
{
    vec4 pixel = texture(tex, v_texcoord);

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
