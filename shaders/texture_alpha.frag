#version 440

layout(location = 0) in vec2 v_texcoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D tex;

void main()
{
    vec4 channels = texture(tex, v_texcoord);

    fragColor = vec4(channels.a, channels.a, channels.a, channels.a);
}
