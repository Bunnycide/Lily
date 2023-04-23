//
// Created by jae on 22/04/23.
//

#include "MyApp.h"

void MyApp::draw(long delta) {
    renderPass.beginRenderPass();
    renderPass.recordCommandBuffer();
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
}


