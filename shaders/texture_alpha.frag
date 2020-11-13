#version 440

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
    float pixel = texture(tex, v_texcoord).a;

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
