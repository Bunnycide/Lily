#version 450

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 tex_attr;

layout(binding = 1) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 incolor;
layout (location = 2) in vec2 attr;

void main() {
    gl_Position = ubo.model * vec4(pos, 1.0);
    fragColor = incolor;
    tex_attr = attr;
}
