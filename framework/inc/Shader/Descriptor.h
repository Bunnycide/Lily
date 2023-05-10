//
// Created by jae on 24/04/23.
//

#ifndef VK_FRAMEWORK_IMPL_DESCRIPTOR_H
#define VK_FRAMEWORK_IMPL_DESCRIPTOR_H


#include "common/lily_structs.h"
#include "Device/Device.h"

class Descriptor {
    VkVertexInputBindingDescription                 vertex_input_binding_description;
    std::vector<VkVertexInputAttributeDescription>  attribute_descriptions;

    VkDescriptorPool                                descriptor_pool;
    std::vector<VkDescriptorSetLayout>              layouts;
    std::vector<VkDescriptorSet>                    descriptor_sets;

    void init(const std::vector<uint32_t>&);

    void makeDescriptorData(Device&, const std::vector<uint32_t> &compilerResult);

    void makeVertexBindingData(const std::vector<uint32_t> &compilerResult);
};


#endif //VK_FRAMEWORK_IMPL_DESCRIPTOR_H
