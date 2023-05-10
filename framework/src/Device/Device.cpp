//
// Created by jae on 20/04/23.
//

#include <Device/Device.h>
#include <Log/Log.h>
#include <common/vk_layer_and_extension_utils.h>
#include <Device/utils/vk_instance_and_device_utils.h>
#include "common/lily_macros.h"

Device::Device(WindowBase& mwindow){

    if(! loadVulkan()){
        Log::info("Failed to load vulkan");
        return;
    }

    if(Config::ENABLE_VALIDATION_LAYERS && ! H_checkValidationLayerSupport()){
        Log::error("Validation layers not found");
        return;
    }

//    if(Config::ENABLE_VALIDATION_LAYERS){
//        H_setupDebugMessenger(instance, debugMessenger);
//    }

    ContextType contextType;

    if(! createInstance(contextType)){
        Log::error("Failed to create vulkan instance");
        return;
    }

    mwindow.createWindowSurface(instance);

    if(!  pickPhysicalDevice()){
        Log::error("Could not find suitable physical device");
        return;
    }

    if(!fetchRenderSurfaceFormats(mwindow.surface)){
        Log::error("Could not find suitable surface formats");
        return;
    }

    if(! createLogicalDevice()){
        Log::error("Could not find physical device");
        return;
    }

    allocateCommandPools();
}

bool Device::createInstance(ContextType contextType) {
    if(!H_areInstanceExtensionsSupported()){
        Log::error("Instance level extensions not supported");
        return false;
    }

    // Vulkan instance creation and func loading
    H_createVulkanInstance(instance, contextType);

    if(! loadInstanceVulkanFunctions(instance)){
        Log::error("Failed to load instance level vulkan funcs");
        return false;
    }

    // Load instance level funcs from extensions
    std::vector<const char*> instanceExtensions(Consts::INSTANCE_EXTENSIONS, Consts::INSTANCE_EXTENSIONS+Consts::NUM_INSTANCE_EXTENSIONS);
    if(! loadInstanceVulkanFunctionsFromExtensions(instance, instanceExtensions)){
        Log::error("Failed to load instance level vulkan funcs from extensions");
        return false;
    }

    return true;
}

bool Device::pickPhysicalDevice() {
    std::vector<VkPhysicalDevice> physicalDevices;

    H_findAllPhysicalDevices(instance ,physicalDevices);
    H_pickSuitablePhysicalDevice(physicalDevice, physicalDevices);

    if(! H_areDeviceExtensionsSupported(physicalDevice)){
        Log::error("Could not load desired device extensions");
        return false;
    }

    // Get physical device memory properties for later
    vkGetPhysicalDeviceMemoryProperties(physicalDevice,
                                        &physicalDeviceMemoryProperties);

    return true;
}

bool Device::createLogicalDevice() {
    H_createLogicalDevice(logicalDevice, physicalDevice,
                          graphicsQueueSet,
                          transferQueueSet,
                          computeQueueSet);

    // Load device level vulkan functions
    if(!loadDeviceVulkanFunctions(logicalDevice)){
        Log::error("Failed to load device level vulkan funcs");
        return false;
    }

    // Load device level vulkan functions from extensions
    std::vector<const char*> deviceExtensions(Consts::DEVICE_EXTENSIONS, Consts::DEVICE_EXTENSIONS+Consts::NUM_DEVICE_EXTENSIONS);
    if(! loadDeviceVulkanFunctionsFromExtensions(logicalDevice, deviceExtensions)){
        Log::error("Failed to load device level vulkan funcs from extensions");
        return false;
    }

    // Retrieve required device queues
    H_retrieveDeviceQueues(logicalDevice,
                           graphicsQueueSet,
                           transferQueueSet,
                           computeQueueSet);

    return true;
}

bool Device::allocateCommandPools() {
    graphicsCommands.allocate(logicalDevice, 2, graphicsQueueSet.queueFamilyIndex);
    computeCommands.allocate(logicalDevice, 2, computeQueueSet.queueFamilyIndex);
    transferCommands.allocate(logicalDevice, 2, transferQueueSet.queueFamilyIndex);

    return true;
}

bool Device::fetchRenderSurfaceFormats(VkSurfaceKHR& surface) {
    uint32_t count = 0;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                                         surface,
                                                         &count, nullptr))

    if(count <= 0){
        Log::info("Found no surface formats");
        windowSurfaceFormats.resize(0);
        return false;
    }

    windowSurfaceFormats.resize(count);
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,
                                                         surface,
                                                         &count, windowSurfaceFormats.data()))


    return true;
}
