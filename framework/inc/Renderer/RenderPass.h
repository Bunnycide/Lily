//
// Created by jae on 22/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_RENDERPAS_H
#define VK_FRAMEWORK_IMPL_RENDERPAS_H

#include <array>
#include "Device/Device.h"
#include "Window/plat_window.h"
#include "Renderer/Renderer.h"
#include "Resources/Buffer/VertexBuffer.h"
#include "Commands/Commands.h"

class RenderPass {
private:
    std::vector<VkFence> inFlightFences;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;

    std::vector<WaitSemaphoreInfo>  waitSemaphoreInfos;
    std::vector<VkSemaphore>        signalSemaphoreInfos;

    std::vector<VkCommandBuffer>    submitCommandBuffers;

    Commands commands;
    VkCommandBuffer currentCommandBuffer;
    std::array<VkClearValue, 2> clearValues;

    VkQueue gfxQueue;
    uint32_t gfxQueueIndex = 0;

    uint32_t current_frame = 0;
    uint32_t swapChainImageIndex = 0;

    uint32_t pass = 0;

    VkRenderPassBeginInfo rpInfo;
    VkPresentInfoKHR presentInfo;
    VkSubpassDependency dependency;

    Window mWindow;
    VkDevice logicalDevice;
    VulkanSwapChain mPresentation;
    VulkanRender mRenderData;

public:
    uint32_t max_frames_in_flight = 2;

    void init(Device&, Window&, Renderer&);

    void beginRenderPass();
    void endRenderPass();

    void present();

    void setScissorRect();
    void setViewPort();
    void bindVertexBuffer(VertexBuffer& buffer);
    void bindIndexBuffer(VertexBuffer& vertexBuffer);
    void drawIndexed();
    void drawElement();

    void recordCommandBuffer();
};


#endif //VK_FRAMEWORK_IMPL_RENDERPAS_H
