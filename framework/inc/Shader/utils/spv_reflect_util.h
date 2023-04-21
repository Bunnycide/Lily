//
// Created by jae on 16/04/23.
//

#include <Shader/utils/spirv_reflect.h>
#include <vk_loader/vk_loader.h>
#include <common/lily_structs.h>

std::vector<DescriptorSetLayoutData>
generateDescriptorInfo(const std::vector<uint32_t>&);

VertexInfo
generateVertexInfo(const std::vector<uint32_t>&);

VkPipelineVertexInputStateCreateInfo
createVertexStateInfo(const std::vector<uint32_t>&);
