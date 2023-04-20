//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_COMMANDS_H
#define VK_FRAMEWORK_IMPL_COMMANDS_H

#include <vk_loader/vk_loader.h>
#include <common/lily_structs.h>

class Commands {
public:
    Commands() = default;

    CommandPoolInfo commandPoolInfo;

    void allocate(VkDevice, uint32_t, uint32_t);
};


#endif //VK_FRAMEWORK_IMPL_COMMANDS_H
