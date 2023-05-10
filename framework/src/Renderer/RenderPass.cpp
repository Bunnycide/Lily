//
// Created by jae on 22/04/23.
//

#include "Renderer/RenderPass.h"
#include "Sync/semaphores/vk_semaphore_utils.h"
#include "Sync/fences/vk_fence_utils.h"
#include "common/lily_enums.h"
#include "Commands/utils/vk_command_X_utils.h"

void RenderPass::setScissorRect() {

}

void RenderPass::setViewPort() {

}

void RenderPass::bindVertexBuffer(BufferInfo &buffer, VkDeviceSize offset) {
    offset = 0;
    vkCmdBindVertexBuffers(currentCommandBuffer,
                           0,
                           1,
                           &buffer.buffer,
                           &offset);
}

void RenderPass::bindIndexBuffer(BufferInfo& bufferInfo) {
    vkCmdBindIndexBuffer(currentCommandBuffer,
                         bufferInfo.buffer,
                         0,
                         VK_INDEX_TYPE_UINT16);
}

void RenderPass::drawIndexed(uint32_t size) {
    vkCmdDrawIndexed(currentCommandBuffer,
                     size,
                     1,
                     0,
                     0,
                     0);
}

void RenderPass::init(Device& device,
                       Window& window,
                       Renderer& renderer) {
    mWindow = window;
    logicalDevice = device.logicalDevice;
    mPresentation = renderer.swapChain;
    mRenderData = renderer.render;

    inFlightFences                    .resize(max_frames_in_flight);
    imageAvailableSemaphores                .resize(max_frames_in_flight);
    renderFinishedSemaphores                 .resize(max_frames_in_flight);

    for(int i = 0; i < max_frames_in_flight; i ++){
        H_createFence(device.logicalDevice, true, inFlightFences[i]);
        H_createSemaphore(device.logicalDevice, imageAvailableSemaphores[i]);
        H_createSemaphore(device.logicalDevice, renderFinishedSemaphores[i]);
    }

    gfxQueue = device.graphicsQueueSet.queues[0];
    gfxQueueIndex = device.graphicsQueueSet.queueFamilyIndex;

    commands.allocate(  device.logicalDevice,
                        max_frames_in_flight,
                        gfxQueueIndex);
    mDescriptorData = renderer.descriptorData;
}

void RenderPass::beginRenderPass() {
    currentCommandBuffer = commands.commandPoolInfo.commandBuffers[current_frame];

    H_waitForFence(logicalDevice, ONE_SEC, inFlightFences[current_frame]);
    H_resetFence(logicalDevice, inFlightFences[current_frame]);

    vkAcquireNextImageKHR(logicalDevice,
                          mPresentation.swapchain,
                          UINT64_MAX,
                          imageAvailableSemaphores[current_frame],
                          VK_NULL_HANDLE,
                          &swapChainImageIndex);
}

void RenderPass::recordCommandBuffer(){
    H_resetCommandBuffer(currentCommandBuffer,
                         VK_FALSE);

    H_beginCommandBufferRecording(currentCommandBuffer,
                                  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    clearValues[0] = {{{0.7f, 0.7f, 0.0, 1.0f}}};
    clearValues[1].depthStencil = {1.0f, 0};

    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpInfo.pNext = nullptr;

    rpInfo.renderPass = mRenderData.renderPass;
    rpInfo.renderArea.offset.x = 0;
    rpInfo.renderArea.offset.y = 0;
    rpInfo.renderArea.extent = VkExtent2D {static_cast<uint32_t>(mWindow.width),
                                           static_cast<uint32_t>(mWindow.height)};
    rpInfo.framebuffer = mPresentation.swapChainFrameBuffers[swapChainImageIndex];

    //connect clear values
    rpInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    rpInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(currentCommandBuffer,
                         &rpInfo,
                         VK_SUBPASS_CONTENTS_INLINE);

    viewport.x          = 0.0f;
    viewport.y          = 0.0f;
    viewport.width      = static_cast<float>(mWindow.width);
    viewport.height     = static_cast<float>(mWindow.height);
    viewport.minDepth   = 0.0f;
    viewport.maxDepth   = 1.0f;

    vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);

    scissor.offset = {0, 0};
    scissor.extent = {
            static_cast<uint32_t>(mWindow.width),
            static_cast<uint32_t>(mWindow.height)
    };

    vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

    vkCmdBindDescriptorSets(
/*VkCommandBuffer        commandBuffer,      */ currentCommandBuffer,
/*VkPipelineBindPoint    pipelineBindPoint,  */ VK_PIPELINE_BIND_POINT_GRAPHICS,
/*VkPipelineLayout       layout,             */ mRenderData.pipelineLayout,
/*uint32_t               firstSet,           */ 0,
/*uint32_t               descriptorSetCount, */ static_cast<uint32_t>(mDescriptorData.descriptorSets.size()),
/*const VkDescriptorSet* pDescriptorSets,    */ mDescriptorData.descriptorSets.data(),
/*uint32_t               dynamicOffsetCount, */ 0,
/*const uint32_t*        pDynamicOffsets     */ nullptr
    );

    vkCmdBindPipeline( currentCommandBuffer,
                       VK_PIPELINE_BIND_POINT_GRAPHICS,
                       mRenderData.gfxPipeline);

}

void RenderPass::endRenderPass() {
    vkCmdEndRenderPass(currentCommandBuffer);
    H_endCommandBufferRecording(currentCommandBuffer);

    waitSemaphoreInfos = {{
                                  imageAvailableSemaphores[current_frame],
                                  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
                          }};

    signalSemaphoreInfos = { renderFinishedSemaphores[current_frame] };

    submitCommandBuffers = { currentCommandBuffer };

    H_submitCommandBufferToQueue(gfxQueue,
                                 waitSemaphoreInfos,
                                 signalSemaphoreInfos,
                                 submitCommandBuffers,
                                 inFlightFences[current_frame]);
}

void RenderPass::present() {

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphoreInfos.data();

    VkSwapchainKHR swapChains[] = {mPresentation.swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &swapChainImageIndex;

    vkQueuePresentKHR(gfxQueue, &presentInfo);

    current_frame = (current_frame + 1) % max_frames_in_flight;
}

void RenderPass::updateRenderer(Renderer& renderer) {
    mPresentation = renderer.swapChain;
    mRenderData = renderer.render;
    mDescriptorData = renderer.descriptorData;
}
