//
// Created by jae on 12/04/23.
//

#include <Sync/fences/vk_fence_utils.h>
#include <common/lily_macros.h>

void H_createFence(VkDevice logicalDevice,
                   VkBool32 signaled,
                   VkFence& fence){

    VkFenceCreateInfo fenceCreateInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};

    fenceCreateInfo.pNext = nullptr;
    fenceCreateInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    VK_CHECK_RESULT(vkCreateFence(logicalDevice, &fenceCreateInfo, nullptr, & fence));
}

void H_waitForFences(VkDevice logicalDevice,
                     VkBool32 waitForAll ,
                     uint64_t timeOut,
                     std::vector<VkFence> fences){
    VK_CHECK_RESULT(vkWaitForFences(logicalDevice,
                                    static_cast<uint32_t>(fences.size()),
                                    fences.data(),
                                    waitForAll,
                                    timeOut))
}

void H_resetFences(VkDevice logicalDevice,
                   std::vector<VkFence>& fences){
    VK_CHECK_RESULT(vkResetFences(logicalDevice,
                                  static_cast<uint32_t>(fences.size()),
                                  &fences[0]))
}

void H_waitForFence(VkDevice logicalDevice,
                    uint64_t timeOut,
                    VkFence fence){
    VK_CHECK_RESULT(vkWaitForFences(logicalDevice,
                                    1,
                                    &fence,
                                    true,
                                    timeOut))
}

void H_resetFence(VkDevice logicalDevice,
                   VkFence fence){
    VK_CHECK_RESULT(vkResetFences(logicalDevice,
                                  1,
                                  &fence));

}

void H_destroyFence(VkDevice logicalDevice,
                    VkFence fence){
    vkDestroyFence(logicalDevice, fence, nullptr);
}
