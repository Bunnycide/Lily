//
// Created by jae on 20/04/23.
//

#include <Resources/Buffer.h>

Buffer::Buffer(Device device,
               VkDeviceSize sz,
               VkBufferUsageFlags usage,
               VkMemoryPropertyFlagBits memoryProperties) {
    bufferInfo.bufSz = sz;
    bufferInfo.memoryProperties = memoryProperties;
    bufferInfo.usage = usage;

    H_createBuffer(device.logicalDevice,
                   bufferInfo);

    H_allocateAndBindMemoryObjectToBuffer(device.physicalDevice,
                                          device.logicalDevice,
                                          device.physicalDeviceMemoryProperties,
                                          bufferInfo);
}

void Buffer::free(Device device){
    H_freeBuffer(device.logicalDevice, bufferInfo);
}
