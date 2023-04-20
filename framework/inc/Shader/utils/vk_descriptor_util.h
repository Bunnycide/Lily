//
// Created by jae on 16/04/23.
//

#ifndef VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H
#define VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H

#include <common/lily_structs.h>


void
H_allocateDescriptorSets(VkDevice ,
                              VkDescriptorPool& ,
                              const std::vector<VkDescriptorSetLayout>& ,
                              std::vector<VkDescriptorSet>& );

void
H_createDescriptorSetLayout(VkDevice ,
                                 const std::vector<VkDescriptorSetLayoutBinding>& ,
                                 VkDescriptorSetLayout& );

std::vector<VkDescriptorPoolSize>
H_extractDescriptorSetTypes(std::vector<VkDescriptorSetLayoutBinding>& );

void
H_createDescriptorPool(VkDevice,
                       const std::vector<VkDescriptorPoolSize>&,
                       uint32_t,
                       VkDescriptorPoolCreateFlags,
                       VkDescriptorPool&);

void
H_updateDescriptorSets(VkDevice ,
                       const std::vector<BufferDescriptorInfo>&);

                       void
H_updateDescriptorSets(VkDevice,
                       const std::vector<ImageDescriptorInfo>& );

void
H_updateDescriptorSets(VkDevice ,
                       const std::vector<BufferDescriptorInfo>& ,
                       const std::vector<CopyDescriptorInfo>& );

void
H_updateDescriptorSets(VkDevice ,
                       const std::vector<ImageDescriptorInfo>& ,
                       const std::vector<CopyDescriptorInfo>& );

void
H_destroyDescriptorData(VkDevice ,
                        DescriptorData &);

#endif //VK_FRAMEWORK_VK_DESCRIPTOR_UTIL_H
