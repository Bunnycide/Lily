//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_DEVICE_H
#define VK_FRAMEWORK_IMPL_DEVICE_H

#include <Window/WindowBase.h>
#include <common/lily_structs.h>
#include <Commands/Commands.h>

class Device {
private:
    bool createInstance(ContextType contextType);
    bool pickPhysicalDevice();
    bool createLogicalDevice();
    bool allocateCommandPools();
    bool fetchRenderSurfaceFormats(VkSurfaceKHR&);

public:
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;
    std::vector<VkSurfaceFormatKHR> windowSurfaceFormats;

    QueueInfo graphicsQueueSet;
    QueueInfo transferQueueSet;
    QueueInfo computeQueueSet;

    Commands graphicsCommands;
    Commands transferCommands;
    Commands computeCommands;

    VkInstance          instance;
    VkPhysicalDevice    physicalDevice;
    VkDevice            logicalDevice;

    explicit Device(WindowBase&);
    ~Device() = default;
};


#endif //VK_FRAMEWORK_IMPL_DEVICE_H
