//
// Created by jae on 22/04/23.
//

#include "MyApp.h"

void MyApp::draw(long delta) {
    renderPass.beginRenderPass();
    renderPass.recordCommandBuffer();
    renderPass.bindIndexBuffer(indexBuffer.bufferInfo);
    renderPass.bindVertexBuffer(vertexBuffer.bufferInfo, 0);
    renderPass.drawIndexed(6);
    renderPass.endRenderPass();
    renderPass.present();
}

void MyApp::run() {

    while(! mWindow.isWindowClosing()){
        draw(0.0l);
        mWindow.updateWindow();
    }
}

MyApp::MyApp(Device &device, Window &window) : mDevice(device), mWindow(window){
    shaderBuilder.addShader(mDevice,
                            "/shaders/shader.vert",
                            VK_SHADER_STAGE_VERTEX_BIT)
            ->addShader(mDevice,
                        "/shaders/shader.frag",
                        VK_SHADER_STAGE_FRAGMENT_BIT);

    renderer.init(mDevice, mWindow, shaderBuilder);

    renderPass.init(device, window, renderer);

    std::vector<Vertex> squareVertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{1.0f,  -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{1.0f,   1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-1.0f,  1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };

    vertexBuffer.init(device, squareVertices.size() * sizeof (Vertex), squareVertices.data());

    std::vector<uint16_t> squareIndices = {
            0, 1, 2, 2, 3, 0
    };

    indexBuffer.init(device, squareIndices.size() * sizeof(uint16_t), squareIndices.data());

    Texture2D texture2D(device, "/images/test.jpg");
    texture2D.updateDescriptorInfo(device.logicalDevice,
                                   renderer.descriptorData.descriptorSets[0],
                                   0, 0);

    CameraMVP camera;
                                            
    glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.0f);
    camera.model = glm::mat4(1.0f);
    camera.model = glm::scale(camera.model, scale);
    
    camera.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    camera.proj = glm::perspective(glm::radians(45.0f), (float) mWindow.width / (float) mWindow.height, 0.1f, 10.0f);
                                    
    camera.proj[1][1] *= -1;
                                            
    uniformBuffer.init(device, sizeof(camera));

    uniformBuffer.copyToUniformBuffer(device.logicalDevice,
                                      sizeof(CameraMVP),
                                      &camera);
                                                                
    uniformBuffer.updateDescriptors(device.logicalDevice,
                                    renderer.descriptorData.descriptorSets[0],
                                    1, 0);
    
    renderPass.updateRenderer(renderer);
}
