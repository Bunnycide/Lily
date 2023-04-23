//
// Created by jae on 20/04/23.
//

#include <Window/plat_window.h>
#include <Window/vk_render_surface.h>

void Window::createWindow(int width, int height){

    Window::width = width;
    Window::height = height;
    this->width = width;
    this->height = height;

    createWindow(width, height, "lily-window");
}

void Window::createWindow(int width, int height, const char* name){
    pWindow = H_createWindow(width, height, name);
}

void Window::createWindowSurface (VkInstance instance){
    H_createRenderSurface(instance, surface, pWindow);
}

bool Window::isWindowClosing(){
    return glfwWindowShouldClose( pWindow );
}

void Window::updateWindow(){
    glfwSwapBuffers(pWindow);
    glfwPollEvents();
}

void Window::mainLoop(){
    if( pWindow == nullptr ) return;

    while(!glfwWindowShouldClose( pWindow )){

//        pDrawCallBack(0.0l);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }
}
