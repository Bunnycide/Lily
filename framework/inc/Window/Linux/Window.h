//
// Created by jae on 20/04/23.
// Window creation Linux
//

#ifndef VK_FRAMEWORK_IMPL_WINDOW_H
#define VK_FRAMEWORK_IMPL_WINDOW_H

#include <Window/WindowBase.h>
#include <GLFW/glfw3.h>
#include "Window/App.h"

class Window : public WindowBase{

private:
    GLFWwindow* pWindow;

public:
    Window() = default;
    int width, height;

    // Draw callback
    void (*pDrawCallBack)(long) = nullptr;

    void createWindow(int width, int height) override;
    void createWindow(int width, int height, const char* name);

    void createWindowSurface(VkInstance) override;

    void mainLoop();

    bool isWindowClosing();

    void updateWindow();
};

#endif //VK_FRAMEWORK_IMPL_WINDOW_H
