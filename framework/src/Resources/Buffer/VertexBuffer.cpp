//
// Created by jae on 20/04/23.
//

#include <Resources/Buffer/VertexBuffer.h>

void VertexBuffer::init(Device device, VkDeviceSize sz, void* data)
{
    Buffer::init(   device, sz,
           VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    H_copyToVertexBuffer(device.physicalDevice,
                         device.logicalDevice,
                         device.physicalDeviceMemoryProperties,
                         device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                         device.graphicsQueueSet.queues[0],
                         data,
                         VK_QUEUE_FAMILY_IGNORED,
                         Buffer::bufferInfo);
}
