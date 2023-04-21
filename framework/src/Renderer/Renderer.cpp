//
// Created by jae on 21/04/23.
//

#include <Renderer/Renderer.h>
#include <Renderer/utils/vk_render_X_utils.h>
#include <Resources/utils/depth_util.h>
#include <Renderer/utils/vk_swapchain_utils.h>
#include <array>
#include "vk_loader/vulkan/vk_enum_string_helper.h"
#include <Shader/utils/vk_descriptor_util.h>
#include <common/lily_macros.h>

Renderer::Renderer(Device& device,
                   Window& window,
                   ShaderBuilder& shaderBuilder) {
    createDepthAttachment   (device, window);
    createSwapChain         (device, window);
    setupRenderer           (device, window, shaderBuilder);
}

void Renderer::createDepthAttachment(Device& device, Window& window) {
    H_createDepthResources(device.physicalDevice,
                           device.logicalDevice,
                           device.physicalDeviceMemoryProperties,
                           device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                           device.graphicsQueueSet.queues[0],
                           VkExtent3D{
                                   static_cast<uint32_t>(window.width),
                                   static_cast<uint32_t>(window.height),
                                   1
                           },
                           render.depthResource);
}

void Renderer::createSwapChain(Device& device, Window& window) {
    std::vector<QueueInfo> queueInfos = {
            device.graphicsQueueSet,
            device.transferQueueSet,
            device.computeQueueSet
    };

    H_createSwapChain(device.physicalDevice,
                      device.windowSurfaceFormats,
                      device.logicalDevice,
                      window.surface,
                      window.width,
                      window.height,
                      queueInfos,
                      swapChain.swapchain);

    H_getSwapChainImages(device.logicalDevice,
                         swapChain.swapchain,
                         swapChain.swapChainImages);

    VkSurfaceFormatKHR surfaceFormat =
            H_findPresentSurfaceImageFormat(device.windowSurfaceFormats);

    swapChain.colorSpace = surfaceFormat.colorSpace;
    swapChain.surfaceFormat = surfaceFormat.format;

    H_createSwapChainImageViews(device.logicalDevice,
                                surfaceFormat,
                                swapChain.swapChainImages,
                                swapChain.swapChainImageViews);
}

void Renderer::setupRenderer(Device& device,
                             Window& window,
                             ShaderBuilder& shaderBuilder) {
    std::vector<VkDescriptorSetLayoutCreateInfo> layoutCreateInfo(shaderBuilder.allDescriptorData.size());

    descriptorData.layouts.resize(shaderBuilder.allDescriptorData.size());
    std::vector<VkDescriptorPoolSize> descPoolSizes;

    for(int i =0; i < shaderBuilder.allDescriptorData.size(); i++){
        H_createDescriptorSetLayout(device.logicalDevice,
                                    shaderBuilder.allDescriptorData[i].bindings,
                                    descriptorData.layouts[i]);

        std::vector<VkDescriptorPoolSize> temp =
                H_extractDescriptorSetTypes(shaderBuilder.allDescriptorData[i].bindings);

        for(auto& poolSizes : descPoolSizes){
            descPoolSizes.push_back(poolSizes);
        }
    }

    H_createDescriptorPool(device.logicalDevice,
                           descPoolSizes,
                           shaderBuilder.allDescriptorData.size(),
                           0,
                           descriptorData.descriptorPool);

    H_allocateDescriptorSets(device.logicalDevice,
                             descriptorData.descriptorPool,
                             descriptorData.layouts,
                             descriptorData.descriptorSets);
}
