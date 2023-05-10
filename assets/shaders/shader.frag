#version 400
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
layout (binding = 0) uniform sampler2D tex;
layout (location = 0) in vec2 texcoord;
layout (location = 1) in vec3 outcolor;

layout (location = 0) out vec4 uFragColor;

void main() {
    uFragColor = vec4(0.0f, 0.0f, .00f, 1.0f);
}
