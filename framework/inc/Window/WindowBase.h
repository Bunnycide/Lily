//
// Created by jae on 18/04/23.
//

#ifndef VK_FRAMEWORK_WINDOW_H
#define VK_FRAMEWORK_WINDOW_H

#include <vk_loader/vk_loader.h>

class WindowBase {
public:
    // Window size
    int width = 0, height = 0;

    // Vulkan Window Render Surface
    VkSurfaceKHR surface;

    // Window creation logic
    virtual void createWindow           (int width, int height) = 0;

    // Window surface creation logic
    virtual void createWindowSurface    (VkInstance)            = 0;

protected:

};

#endif //VK_FRAMEWORK_WINDOW_H
