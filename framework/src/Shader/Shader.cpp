//
// Created by jae on 21/04/23.
//

#include <Shader/ShaderBuilder.h>
#include <Shader/utils/shader_utils.h>

void Shader::createShader(std::string shaderPath, VkShaderStageFlags stage) {
    std::string shaderSource;
    getShaderString(shaderPath, shaderSource);
    mstage = stage;
    mspv = compileShaderToSpv(shaderSource, mstage);
}
