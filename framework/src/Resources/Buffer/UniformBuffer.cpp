//
// Created by jae on 27/04/23.
//

#include "Resources/Buffer/UniformBuffer.h"
#include "Shader/utils/vk_descriptor_util.h"

void UniformBuffer::init(Device & device, VkDeviceSize sz)
{
    Buffer::init(device, sz,
           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

    VK_CHECK_RESULT(
            vkMapMemory(device.logicalDevice,
                        Buffer::bufferInfo.memory_object,
                        0,
                        Buffer::bufferInfo.size,
                        0,
                        &Buffer::bufferInfo.memory_pointer))
}

void UniformBuffer::copyToUniformBuffer(VkDevice logical_device, VkDeviceSize offset, void * data) {
    H_copyDataToMappedBuffer(logical_device, Buffer::bufferInfo, offset, data);
}

void UniformBuffer::updateDescriptors(VkDevice logicalDevice, VkDescriptorSet descriptorSet,
                                      uint32_t targetDescriptorBinding, uint32_t targetArrayElement) {
    std::vector<VkDescriptorBufferInfo> bufferDescriptorInfos = {
            {
/* VkBuffer     */ .buffer = bufferInfo.buffer,
/* VkDeviceSize */ .offset = 0,
/* VkDeviceSize */ .range  = bufferInfo.size,
            }
    };

    std::vector<BufferDescriptorInfo> descriptorInfos= {
            BufferDescriptorInfo{
/* VkDescriptorSet                      */ .TargetDescriptorSet     = descriptorSet,
/* uint32_t                             */ .TargetDescriptorBinding = targetDescriptorBinding,
/* uint32_t                             */ .TargetArrayElement      = targetArrayElement,
/* VkDescriptorType                     */ .TargetDescriptorType    = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
/* std::vector<VkDescriptorBufferInfo>  */ .BufferInfos             = bufferDescriptorInfos,
            }
    };

    H_updateDescriptorSets(logicalDevice, descriptorInfos);
}
