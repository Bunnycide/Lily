//
// Created by jae on 20/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_BUFFER_H
#define VK_FRAMEWORK_IMPL_BUFFER_H

#include <common/lily_structs.h>
#include <Resources/utils/vk_buffer_util.h>
#include <Device/Device.h>

class Buffer {

protected:
    BufferInfo bufferInfo;
public:
    Buffer(Device,
           VkDeviceSize,
           VkBufferUsageFlags,
           VkMemoryPropertyFlagBits );

    void free(Device);
};

#endif //VK_FRAMEWORK_IMPL_BUFFER_H
