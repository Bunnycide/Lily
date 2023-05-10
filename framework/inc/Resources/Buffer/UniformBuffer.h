//
// Created by jae on 27/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_UNIFORMBUFFER_H
#define VK_FRAMEWORK_IMPL_UNIFORMBUFFER_H

#include <Resources/Buffer.h>

class UniformBuffer : public Buffer{
public:
    void init(Device& , VkDeviceSize);
    void copyToUniformBuffer(VkDevice, VkDeviceSize, void* );
    void updateDescriptors( VkDevice logicalDevice,
                            VkDescriptorSet descriptorSet,
                            uint32_t targetDescriptorBinding,
                            uint32_t targetArrayElement);
};



#endif //VK_FRAMEWORK_IMPL_UNIFORMBUFFER_H
