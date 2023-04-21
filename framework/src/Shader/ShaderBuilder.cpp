//
// Created by jae on 21/04/23.
//

#include <Shader/ShaderBuilder.h>
#include <memory>
#include <Shader/utils/shader_utils.h>
#include <Shader/utils/vk_descriptor_util.h>
#include <Shader/utils/spv_reflect_util.h>

ShaderBuilder* ShaderBuilder::addShader(Device device, std::string source, VkShaderStageFlags stage) {
    Shader shader = Shader();
    VkShaderModule module;

    shader.createShader(source, stage);
    buildShaderFromSource(device.logicalDevice,
                          shader.mspv,
                          &module);

    shaderModules.push_back(module);

    vertexInfo = generateVertexInfo(shader.mspv);
    descriptorData = generateDescriptorInfo(shader.mspv);

    return this;
}
