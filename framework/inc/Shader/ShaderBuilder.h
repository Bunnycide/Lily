//
// Created by jae on 21/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_SHADERBUILDER_H
#define VK_FRAMEWORK_IMPL_SHADERBUILDER_H

#include "Shader.h"
#include "Device/Device.h"
#include <string>
#include <memory>

class ShaderBuilder {
public:
    ShaderBuilder() = default;
    VertexInfo vertexInfo;
    std::vector<DescriptorSetLayoutData> allDescriptorData;

    std::vector<VkShaderModule> shaderModules;

    ShaderBuilder* addShader(Device, std::string source, VkShaderStageFlags stage);
};


#endif //VK_FRAMEWORK_IMPL_SHADERBUILDER_H
