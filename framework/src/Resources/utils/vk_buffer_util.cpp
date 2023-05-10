#include "Resources/utils/vk_buffer_util.h"
#include "Commands/utils/vk_command_X_utils.h"

#include <stdio.h>
#include <string.h>
#include <cstring>
#include <memory>

void H_createBuffer(VkDevice logicalDevice, BufferInfo& bufferInfo){
    VkBufferCreateInfo createInfo{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};

    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.size = bufferInfo.size;
    createInfo.usage = bufferInfo.usage;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.pQueueFamilyIndices = nullptr;

    VK_CHECK_RESULT(vkCreateBuffer(logicalDevice, &createInfo, nullptr, &bufferInfo.buffer))
}

void H_allocateAndBindMemoryObjectToBuffer(VkPhysicalDevice physicalDevice,
                                           VkDevice logicalDevice,
                                           VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                                           BufferInfo& bufferInfo){

    // Required type of memory for the buffer
    VkMemoryRequirements memory_requirements;
    vkGetBufferMemoryRequirements(logicalDevice, bufferInfo.buffer, &memory_requirements);

    // Initialize memory handle
    bufferInfo.memory_object = VK_NULL_HANDLE;

    for(uint32_t type = 0; type < physicalDeviceMemoryProperties.memoryTypeCount; ++type){

        /**
         *First section to decide if type index is correct
         *(memory_requirements.memoryTypeBits & (1 << type))
         *
         * Second section checks for memory type being present
         *
         * ((physical_device_memory_properties.memoryTypes[type].propertyFlags &
         *    vertexBufferInfo.memoryProperties) == vertexBufferInfo.memoryProperties)
         */

        if( (memory_requirements.memoryTypeBits & (1 << type)) &&
            ((physicalDeviceMemoryProperties.memoryTypes[type].propertyFlags &
              bufferInfo.memory_properties) == bufferInfo.memory_properties) ){

            VkMemoryAllocateInfo buffer_memory_allocate_info = {
                    VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                    nullptr,
                    memory_requirements.size,
                    type
            };

            VkResult result = vkAllocateMemory(logicalDevice, &buffer_memory_allocate_info, nullptr, &bufferInfo.memory_object);

            if(result == VK_SUCCESS){
                break;
            }
        }
    }

    if(VK_NULL_HANDLE == bufferInfo.memory_object){
        Log::error("Could not allocate memory for buffer");
        return;
    }

    VK_CHECK_RESULT(vkBindBufferMemory(logicalDevice, bufferInfo.buffer, bufferInfo.memory_object, 0))
}

void H_mapBuffer(VkDevice logical_device,
                 BufferInfo buffer_info,
                 VkDeviceSize offset){

    VK_CHECK_RESULT(
            vkMapMemory(logical_device,
                        buffer_info.memory_object,
                        offset,
                        buffer_info.size,
                        0,
                        &buffer_info.memory_pointer))
}

void H_copyDataToMappedBuffer(VkDevice logical_device,
                              BufferInfo buffer_info,
                              VkDeviceSize offset,
                              void* data){
    std::memcpy(buffer_info.memory_pointer, data, buffer_info.size);

}

void H_mapAndCopyData(VkDevice logical_device,
                      BufferInfo buffer_info,
                      VkDeviceSize offset,
                      void* data){
    H_mapBuffer(logical_device, buffer_info, offset);
    H_copyDataToMappedBuffer(logical_device, buffer_info, offset, data);

    std::vector<VkMappedMemoryRange> memory_ranges = {
            {
                    VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                    nullptr,
                    buffer_info.memory_object,
                    offset,
                    buffer_info.size
            }
    };

    VK_CHECK_RESULT(
            vkFlushMappedMemoryRanges(logical_device,
                                      1,
                                      &memory_ranges[0]))
}

void H_setBufferMemoryBarrier(std::vector<BufferTransition> &bufferTransitions,
                              VkPipelineStageFlags generatingStages,
                              VkPipelineStageFlags consumingStages,
                              VkQueue queue,
                              VkCommandBuffer commandBuffer){
    std::vector<VkBufferMemoryBarrier> buffer_memory_barriers;

    for(auto& bufferTransition : bufferTransitions){
        buffer_memory_barriers.push_back({
            VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
            nullptr,
            bufferTransition.currentAccess,
            bufferTransition.newAccess,
            bufferTransition.currentQueueFamily,
            bufferTransition.newQueueFamily,
            bufferTransition.buffer,
            0,
            VK_WHOLE_SIZE
        });
    }

    H_beginCommandBufferRecording(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdPipelineBarrier(commandBuffer,
                         generatingStages,
                         consumingStages,
                         0, 0,
                         nullptr,
                         static_cast<uint32_t>(buffer_memory_barriers.size()),
                         &buffer_memory_barriers[0],
                         0,
                         nullptr);

    H_endCommandBufferRecording(commandBuffer);

    VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer
    };

    vkQueueSubmit(queue,
                  1,
                  &submitInfo,
                  VK_NULL_HANDLE);
    
    vkQueueWaitIdle(queue);
}

void H_copyDataUsingStagingBuffer(VkCommandBuffer   command_buffer,
                                  VkQueue           queue,
                                  BufferInfo        source_buffer,
                                  BufferInfo        destination_buffer){
    std::vector<VkBufferCopy> regions = {
            {
                0,0,source_buffer.size
            }
    };

    H_beginCommandBufferRecording(command_buffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdCopyBuffer(command_buffer,
                    source_buffer.buffer,
                    destination_buffer.buffer,
                    static_cast<uint32_t>(regions.size()),
                    &regions[0]);

    H_endCommandBufferRecording(command_buffer);

    std::vector<VkSubmitInfo> queue_submit_infos = {
            VkSubmitInfo{
                .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .commandBufferCount = 1,
                .pCommandBuffers    = &command_buffer
            }
    };

    vkQueueSubmit(queue,
                  static_cast<uint32_t>(queue_submit_infos.size()),
                  queue_submit_infos.data(),
                  nullptr);

    vkQueueWaitIdle(queue);
}

void H_createBufferView(VkDevice logicalDevice,
                        VkBuffer buffer,
                        VkFormat bufferFormat,
                        VkDeviceSize memoryOffset,
                        VkDeviceSize memoryRange,
                        VkBufferView &bufferView){
    VkBufferViewCreateInfo bufferViewCreateInfo{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};

/* const void*             */ bufferViewCreateInfo.pNext    = nullptr;
/* VkBufferViewCreateFlags */ bufferViewCreateInfo.flags    = 0;
/* VkBuffer                */ bufferViewCreateInfo.buffer   = buffer;
/* VkFormat                */ bufferViewCreateInfo.format   = bufferFormat;
/* VkDeviceSize            */ bufferViewCreateInfo.offset   = memoryOffset;
/* VkDeviceSize            */ bufferViewCreateInfo.range    = memoryRange;

    VK_CHECK_RESULT(vkCreateBufferView(logicalDevice, &bufferViewCreateInfo, nullptr, &bufferView))
}

void H_createVertexBuffer(VkDevice logicalDevice,
                          VkDeviceSize size,
                          BufferInfo& bufferInfo){
    bufferInfo.memory_properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    H_createBuffer(logicalDevice, bufferInfo);
}

void H_createUniformBuffer(VkDevice logicalDevice,
                           VkDeviceSize size,
                           BufferInfo& bufferInfo){
    bufferInfo.memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    H_createBuffer(logicalDevice, bufferInfo);
}

void H_copyToVertexBuffer(VkPhysicalDevice physicalDevice,
                          VkDevice logicalDevice,
                          VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                          VkCommandBuffer commandBuffer,
                          VkQueue queue,
                          void* data,
                          uint32_t queueFamilyIndex,
                          BufferInfo& dstBufferInfo){

    BufferInfo stagingBuffer{};
    stagingBuffer.memory_properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
    stagingBuffer.size = dstBufferInfo.size;
    stagingBuffer.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    H_createBuffer(logicalDevice, stagingBuffer);

    H_allocateAndBindMemoryObjectToBuffer(physicalDevice,
                                          logicalDevice,
                                          physicalDeviceMemoryProperties,
                                          stagingBuffer);

    VK_CHECK_RESULT(vkMapMemory(logicalDevice,
                                stagingBuffer.memory_object,
                                0,
                                stagingBuffer.size,
                                0,
                                &stagingBuffer.memory_pointer))

    memcpy( stagingBuffer.memory_pointer,
            data,
            stagingBuffer.size);

    std::vector<VkBufferCopy> regions;

    regions.push_back(VkBufferCopy{
        0,0,stagingBuffer.size
    });

    H_beginCommandBufferRecording(commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdCopyBuffer(commandBuffer, stagingBuffer.buffer, dstBufferInfo.buffer,
                    static_cast<uint32_t>(regions.size()),
                    &regions[0]);

    H_endCommandBufferRecording(commandBuffer);

    VkSubmitInfo submitInfo{
        .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers    = &commandBuffer
    };

    vkQueueSubmit(queue, 1, &submitInfo, nullptr);

    vkQueueWaitIdle(queue);

//    bufferTransition[0] = {
//            .buffer = dstBufferInfo.buffer,
//            .currentAccess = VK_ACCESS_TRANSFER_WRITE_BIT,
//            .newAccess = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
//            .currentQueueFamily = queueFamilyIndex,
//            .newQueueFamily = queueFamilyIndex
//    };
//
//    H_setBufferMemoryBarrier(bufferTransition,
//                             VK_PIPELINE_STAGE_TRANSFER_BIT,
//                             VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
//                             queue,
//                             commandBuffer);

    H_freeBuffer(logicalDevice, stagingBuffer);
}

void H_freeBuffer(VkDevice logicalDevice, BufferInfo& bufferInfo){
    vkFreeMemory(logicalDevice, bufferInfo.memory_object, nullptr);
    vkDestroyBuffer(logicalDevice, bufferInfo.buffer, nullptr);
}