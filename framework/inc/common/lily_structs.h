#ifndef LILY_VK_LAYER_AND_EXTENSION_UTILS_H
#define LILY_VK_LAYER_AND_EXTENSION_UTILS_H

#include <vk_loader/vk_loader.h>
#include <glm/glm.hpp>
#include <Log/Log.h>

namespace Config {
    constexpr static const bool ENABLE_VALIDATION_LAYERS = true;
}

struct Consts{
    static const int REQUIRED_QUEUE_COUNT  = 3;
    constexpr static const float QUEUE_PRIORITY_HIGHEST = 1.0f;
    constexpr static const float QUEUE_PRIORITY_LOWEST  = 0.0f;

    constexpr static const int NUM_VALIDATION_LAYERS = 1;
    constexpr static const char* VALIDATION_LAYERS[] = {
            "VK_LAYER_KHRONOS_validation",
//            "VK_LAYER_LUNARG_api_dump"
    };

    constexpr static const int NUM_INSTANCE_EXTENSIONS = 3;
    constexpr static const char* INSTANCE_EXTENSIONS[] = {
            VK_KHR_SURFACE_EXTENSION_NAME,
            "VK_KHR_xcb_surface",
            VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
    };

    constexpr static const int NUM_DEVICE_EXTENSIONS = 1;
    constexpr static const char* DEVICE_EXTENSIONS[1] = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
};

struct LightData{

};

struct ContextType {
    bool RAY_TRACING_ENABLED    = false;
    bool COMPUTE_ONLY           = false;
    bool GRAPHICS_ONLY          = true;
    bool GRAPHICS_AND_COMPUTE   = false;
};

struct BufferInfo{
    VkBuffer                        buffer;
    VkDeviceSize                    size;
    VkBufferUsageFlags              usage;
    VkDeviceMemory                  memory_object;
    VkMemoryPropertyFlags           memory_properties;
    void*                           memory_pointer;
};

struct ImageInfo{
    VkImage image;
    VkImageView imageView;
    VkDeviceSize deviceSize;
    VkImageUsageFlags imageUsageFlags;
    VkDeviceMemory memoryObj;
    VkMemoryPropertyFlagBits memoryProperties;
    void* imageData;
    int imgWidth, imgHeight;
    uint32_t imgSize;
};

struct VertexInfo {
    VkVertexInputBindingDescription binding_description;
    std::vector<VkVertexInputAttributeDescription> attribute_descriptions;
};

struct DescriptorSetLayoutData {
    uint32_t set_number;
    VkDescriptorSetLayoutCreateInfo create_info;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
};

struct BufferTransition {
    VkBuffer        buffer;
    VkAccessFlags   currentAccess;
    VkAccessFlags   newAccess;
    uint32_t        currentQueueFamily;
    uint32_t        newQueueFamily;
};

struct DescriptorData{
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSetLayout> layouts;
    std::vector<VkDescriptorSet> descriptorSets;
};

struct ImageTransition {
    VkImage             image;
    VkAccessFlags       currentAccess;
    VkAccessFlags       newAccess;
    VkImageLayout       currentLayout;
    VkImageLayout       newLayout;
    uint32_t            currentQueueFamily;
    uint32_t            newQueueFamily;
    VkImageAspectFlags  aspect;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
};

struct CameraMVP{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct VertexBufferParameters {
    VkBuffer Buffer;
    VkDeviceSize MemoryOffset;
};

struct ImageDescriptorInfo {
    VkDescriptorSet                     TargetDescriptorSet;
    uint32_t                            TargetDescriptorBinding;
    uint32_t                            TargetArrayElement;
    VkDescriptorType                    TargetDescriptorType;
    std::vector<VkDescriptorImageInfo>  imageInfos;
};

struct BufferDescriptorInfo {
    VkDescriptorSet TargetDescriptorSet;
    uint32_t TargetDescriptorBinding;
    uint32_t TargetArrayElement;
    VkDescriptorType TargetDescriptorType;
    std::vector<VkDescriptorBufferInfo> BufferInfos;
};

struct CopyDescriptorInfo {
    VkDescriptorSet TargetDescriptorSet;
    uint32_t TargetDescriptorBinding;
    uint32_t TargetArrayElement;
    VkDescriptorSet SourceDescriptorSet;
    uint32_t SourceDescriptorBinding;
    uint32_t SourceArrayElement;
    uint32_t DescriptorCount;
};

struct ShaderInfo{
    VkShaderModule shaderModule;
    VkShaderStageFlags type;
};

struct QueueInfo{
    std::vector<VkQueue> queues;
    std::vector<float> queuePriorities;
    uint32_t queueFamilyIndex;
    uint32_t queueCount;
};

struct WaitSemaphoreInfo {
    VkSemaphore Semaphore;
    VkPipelineStageFlags WaitingStage;
};

struct VulkanInstance{
    VkInstance                          instance;
    VkDevice                            logicalDevice;
    VkPhysicalDevice                    physicalDevice;
    std::vector<QueueInfo>              queueInfos;
    VkPhysicalDeviceMemoryProperties    physicalDeviceMemoryProperties;
};

struct VulkanSwapChain{
    VkSwapchainKHR swapchain;

    VkSurfaceKHR surface;
    VkFormat surfaceFormat;
    VkColorSpaceKHR colorSpace;
    uint32_t swapChainSize;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFrameBuffers;
};

struct CommandPoolInfo{
    VkCommandPool commandPool;
    uint32_t queueFamilyIndex;
    std::vector<VkCommandBuffer> commandBuffers;
};

struct VulkanRender{
    VkPipeline          gfxPipeline;
    VkRenderPass        renderPass;
    VkPipelineLayout    pipelineLayout;
    ImageInfo           depthResource;
};

#endif ///LILY_VK_LAYER_AND_EXTENSION_UTILS_H