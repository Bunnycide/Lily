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
#include "Resources/utils/vk_image_util.h"

void Renderer::init(Device& device,
                   Window& window,
                   ShaderBuilder& shaderBuilder) {
    createDepthAttachment   (device, window);
    createSwapChain         (device, window);
    setupRenderer           (device, window, shaderBuilder);
}

void Renderer::createDepthAttachment(Device& device, Window& window) {
    depthFormat = H_findDepthFormat(device.physicalDevice);

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

    swapChain.swapChainSize = swapChain.swapChainImages.size();

    surfaceFormat =
            H_findPresentSurfaceImageFormat(device.windowSurfaceFormats);

    swapChain.colorSpace = surfaceFormat.colorSpace;
    swapChain.surfaceFormat = surfaceFormat.format;

    H_createSwapChainImageViews(device.logicalDevice,
                                surfaceFormat,
                                swapChain.swapChainImages,
                                swapChain.swapChainImageViews);

    // Transition swapChain images to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    transitionSwapChainImages(device);
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

        for(auto& poolSizes : temp){
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

    H_createPipelineLayout(device.logicalDevice,
                           render.pipelineLayout,
                           descriptorData.layouts);

    H_createRenderPass(device.logicalDevice,
                       surfaceFormat.format,
                       depthFormat,
                       render.renderPass);

    // Create Swap Chain FrameBuffers

    H_createSwapChainFrameBuffers(  device.logicalDevice,
                                    render.depthResource,
                                    render.renderPass,
                                    swapChain.swapChainImageViews,
                                    swapChain.swapChainFrameBuffers,
                                    window.width,
                                    window.height);

    H_createRenderPipeline(device.logicalDevice,
                           window,
                           shaderBuilder.vertexInfo,
                           shaderBuilder.shaderModules,
                           render.renderPass,
                           render.pipelineLayout,
                           render.gfxPipeline);
}

void Renderer::transitionSwapChainImages(Device & device){
    std::vector<ImageTransition> imageTransitions;

    for(int i = 0; i < swapChain.swapChainSize; i++){
        imageTransitions.push_back(ImageTransition{
            .image = swapChain.swapChainImages[i],
            .currentAccess = VK_ACCESS_NONE_KHR,
            .newAccess= VK_ACCESS_NONE_KHR,
            .currentLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .newLayout =VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .newQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT
        });
    }

    H_setupImageMemoryBarrier(imageTransitions,
                              device.graphicsQueueSet.queues[0],
                              device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                              VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);
}