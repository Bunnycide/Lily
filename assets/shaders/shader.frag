#version 450

layout (binding = 0) uniform sampler2D tex;

layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;

layout(location = 1) in vec2 tex_attr;

void main() {
    vec4 color = texture(tex, tex_attr);
    outColor = vec4(color.xyz, 1.0);
}