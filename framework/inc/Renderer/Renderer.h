//
// Created by jae on 21/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_RENDERER_H
#define VK_FRAMEWORK_IMPL_RENDERER_H

#include <Device/Device.h>
#include <common/lily_structs.h>
#include <Shader/ShaderBuilder.h>

class Renderer {
    VulkanRender render;
    explicit Renderer(Device device, ShaderBuilder builder);
};


#endif //VK_FRAMEWORK_IMPL_RENDERER_H
