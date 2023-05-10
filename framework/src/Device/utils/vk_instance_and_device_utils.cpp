#include <Device/utils/vk_instance_and_device_utils.h>
#include <GLFW/glfw3.h>
#include <common/lily_macros.h>
#include <common/lily_structs.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            Log::info("validation layer INFO : %s", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            Log::info("validation layer VERBOSE : %s", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            Log::info("validation layer WARN : %s", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            Log::info("validation layer ERROR : %s", pCallbackData->pMessage);
            break;
        default:
            Log::info("validation layer: %s", pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

void H_setupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT& messenger){
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT     |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT  |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional

    VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(
                    instance,
                    &createInfo,
                    nullptr,
                    &messenger
            ))
}

void H_createVulkanInstance(VkInstance& instance, ContextType& contextType){
    uint32_t nexts = 0;
    auto exts = glfwGetRequiredInstanceExtensions(&nexts);

    std::vector<const char*> extensions(exts, exts + nexts);

    if(Config::ENABLE_VALIDATION_LAYERS){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    // TODO : Change initialization based on contextType
    VkApplicationInfo applicationInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    applicationInfo.pNext = nullptr;
    applicationInfo.pApplicationName = "vk-framework";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0 , 0);
    applicationInfo.pEngineName = "None";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    instanceCreateInfo.pNext = nullptr;
    instanceCreateInfo.flags = 0;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = Config::ENABLE_VALIDATION_LAYERS ? Consts::NUM_VALIDATION_LAYERS : 0;
    instanceCreateInfo.ppEnabledLayerNames = Config::ENABLE_VALIDATION_LAYERS ? Consts::VALIDATION_LAYERS : nullptr;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

    VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
}

void H_findAllPhysicalDevices(VkInstance& instance,
                              std::vector<VkPhysicalDevice>& physicalDevices) {
    uint32_t physicalDeviceCount = 0;
    VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr))
    if(physicalDeviceCount > 0){
        physicalDevices.resize(physicalDeviceCount);
        VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()))
    }
}

void H_pickSuitablePhysicalDevice(VkPhysicalDevice& physicalDevice,
                                  std::vector<VkPhysicalDevice>& physicalDevices){
    physicalDevice = ! physicalDevices.empty() ? physicalDevices[0] : nullptr;
}

QueueInfo fillQueueInfo(VkQueueFamilyProperties props, uint32_t index){
    QueueInfo queueInfo{};
    queueInfo.queues.resize(props.queueCount);
    queueInfo.queueFamilyIndex = index;
    queueInfo.queueCount = props.queueCount;

    queueInfo.queuePriorities.resize(queueInfo.queueCount, 0.0f);
    float lpriority = 1.0f;

    for(auto& priority : queueInfo.queuePriorities){
        priority = lpriority;
    }

    return queueInfo;
}

void H_createLogicalDevice(VkDevice& device,
                           VkPhysicalDevice& physicalDevice,
                           QueueInfo& graphicsQueueSet,
                           QueueInfo& transferQueueSet,
                           QueueInfo& computeQueueSet){

    uint32_t queueFamilyPropertyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

    for(int i = 0; i < queueFamilyProperties.size(); i++){

        if( queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueSet = fillQueueInfo(queueFamilyProperties[i], i);
            continue;
        }

        if( queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
            computeQueueSet = fillQueueInfo(queueFamilyProperties[i], i);
            continue;
        }

        if( queueFamilyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT ) {
            transferQueueSet = fillQueueInfo(queueFamilyProperties[i], i);
            continue;
        }
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(Consts::REQUIRED_QUEUE_COUNT);
    std::vector<QueueInfo> queueInfos = {   graphicsQueueSet,
                                            transferQueueSet,
                                            computeQueueSet};


    for(int i = 0; i < Consts::REQUIRED_QUEUE_COUNT; i++){
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].pNext = nullptr;
        queueCreateInfos[i].flags = 0;
        queueCreateInfos[i].queueFamilyIndex = queueInfos[i].queueFamilyIndex;
        queueCreateInfos[i].queueCount = queueInfos[i].queueCount;
        queueCreateInfos[i].pQueuePriorities = queueInfos[i].queuePriorities.data();
    }
        
    VkDeviceCreateInfo deviceCreateInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    deviceCreateInfo.pNext  = nullptr;
    deviceCreateInfo.flags  = 0;
    deviceCreateInfo.queueCreateInfoCount   = static_cast<uint32_t>(queueCreateInfos.size());
    deviceCreateInfo.pQueueCreateInfos      = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount      = 0;
    deviceCreateInfo.ppEnabledLayerNames    = nullptr;
    deviceCreateInfo.enabledExtensionCount  = Consts::NUM_DEVICE_EXTENSIONS;
    deviceCreateInfo.ppEnabledExtensionNames = Consts::DEVICE_EXTENSIONS;

    VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device))
}

void H_retrieveDeviceQueues(VkDevice& device,
                            QueueInfo& graphicsQueueInfo,
                            QueueInfo& transferQueueInfo,
                            QueueInfo& computeQueueInfo){

    for(uint32_t i = 0; i < graphicsQueueInfo.queueCount; i ++){
        vkGetDeviceQueue(device, graphicsQueueInfo.queueFamilyIndex, i, &graphicsQueueInfo.queues[i]);
    }

    for(uint32_t i = 0; i < transferQueueInfo.queueCount; i ++){
        vkGetDeviceQueue(device, transferQueueInfo.queueFamilyIndex, i, &transferQueueInfo.queues[i]);
    }

    for(uint32_t i = 0; i < computeQueueInfo.queueCount; i ++){
        vkGetDeviceQueue(device, computeQueueInfo.queueFamilyIndex, i, &computeQueueInfo.queues[i]);
    }
}