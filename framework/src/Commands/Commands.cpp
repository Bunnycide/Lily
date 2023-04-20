//
// Created by jae on 20/04/23.
//

#include <Commands/Commands.h>
#include <Commands/utils/vk_command_X_utils.h>

void Commands::allocate(VkDevice logicalDevice,
                   uint32_t commandBufferCount,
                   uint32_t queueFamilyIndex) {
    // Assign queue family index
    commandPoolInfo.queueFamilyIndex = queueFamilyIndex;

    H_createCommandPool(logicalDevice,
                        commandPoolInfo);

    H_allocateCommandBuffers(logicalDevice,
                             commandBufferCount,
                             commandPoolInfo);
}