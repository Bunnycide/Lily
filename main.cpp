#include <iostream>
#include <Log/Log.h>

#include <Window/Linux/Window.h>
#include <Device/Device.h>

#include <Resources/Buffer/VertexBuffer.h>
#include "Shader/Shader.h"
#include <Shader/ShaderBuilder.h>

void drawFrame(long dealtTime);

void drawFrame(long dealtTime){

}

int main() {

    // Create Window
    Window window;
    window.createWindow(1280, 800);

    // Set draw callback
    window.pDrawCallBack = drawFrame;

    Device device(window);

    ShaderBuilder shaderBuilder;
    shaderBuilder.addShader(device,
                            "/home/jae/CLionProjects/vk-framework-impl/shaders/shader.vert",
                            VK_SHADER_STAGE_VERTEX_BIT)
            ->addShader(device,
                        "/home/jae/CLionProjects/vk-framework-impl/shaders/shader.frag",
                        VK_SHADER_STAGE_FRAGMENT_BIT);

    // Start main loop
    window.mainLoop();

    return 0;
}
