//
// Created by jae on 21/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_RENDERER_H
#define VK_FRAMEWORK_IMPL_RENDERER_H

#include <Device/Device.h>
#include <common/lily_structs.h>
#include <Shader/ShaderBuilder.h>
#include "Window/Linux/Window.h"

class Renderer {
private:
    void createDepthAttachment  (Device&,   Window&);
    void createSwapChain        (Device&,    Window&);
    void setupRenderer          (Device&,    Window&, ShaderBuilder&);
public:
    void init(Device& , Window&, ShaderBuilder& );

    VulkanRender render;
    VulkanSwapChain swapChain;
    DescriptorData descriptorData;

    VkSurfaceFormatKHR  surfaceFormat;
    VkFormat            depthFormat;

    void transitionSwapChainImages(Device &device);
};


#endif //VK_FRAMEWORK_IMPL_RENDERER_H
