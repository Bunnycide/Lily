//
// Created by jae on 21/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_SHADER_H
#define VK_FRAMEWORK_IMPL_SHADER_H

#include <string>
#include <common/lily_structs.h>

class Shader {
public:
    Shader() = default;
    ~Shader() = default;
    VkShaderStageFlags mstage;
    std::vector<uint32_t> mspv;

    void createShader(std::string, VkShaderStageFlags);
};


#endif //VK_FRAMEWORK_IMPL_SHADER_H
