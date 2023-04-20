//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_SHADER_H
#define VK_FRAMEWORK_IMPL_SHADER_H

#include <cstdint>
#include <vk_loader/vk_loader.h>
#include <Device/Device.h>

struct ShaderInfo{
    uint32_t* spv_src;
    VkShaderStageFlags type;
};

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    Shader addModule(const char* sp);
    void finish(Device);
private:
};


#endif //VK_FRAMEWORK_IMPL_SHADER_H
