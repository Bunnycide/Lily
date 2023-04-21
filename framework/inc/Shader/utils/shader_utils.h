//
// Created by jae on 06/04/23.
//

#ifndef VK_FRAMEWORK_VK_SHADER_UTILS_H
#define VK_FRAMEWORK_VK_SHADER_UTILS_H

#include <vk_loader/vk_loader.h>
#include <string>

void getShaderString(const std::string& ,
                     std::string &);

std::vector<uint32_t> compileShaderToSpv(std::string &,
                                         VkShaderStageFlags);

VkResult buildShaderFromSource(VkDevice logicalDevice,
                               const std::vector<uint32_t>&,
                               VkShaderModule* shaderOut);

#endif //VK_FRAMEWORK_VK_SHADER_UTILS_H
