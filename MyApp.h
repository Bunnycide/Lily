//
// Created by jae on 22/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_MYAPP_H
#define VK_FRAMEWORK_IMPL_MYAPP_H

#include <iostream>
#include <Log/Log.h>

#include <Window/plat_window.h>
#include <Device/Device.h>

#include <Resources/Buffer/VertexBuffer.h>
#include <Resources/Buffer/UniformBuffer.h>
#include "Shader/Shader.h"
#include <Shader/ShaderBuilder.h>
#include "Renderer/Renderer.h"
#include "Renderer/RenderPass.h"

#include "Resources/Image/Texture2D.h"
#include "Shader/utils/vk_descriptor_util.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MyApp : public  App{
private:
    ShaderBuilder shaderBuilder;
    Renderer renderer;
    RenderPass renderPass;

    Device mDevice;
    Window mWindow;

    VertexBuffer vertexBuffer;
    VertexBuffer indexBuffer;
    UniformBuffer uniformBuffer;

public:
    MyApp(Device& device, Window& window);

    void draw(long) override;
    void run() override;
};


#endif //VK_FRAMEWORK_IMPL_MYAPP_H
