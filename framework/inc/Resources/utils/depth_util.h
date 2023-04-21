//
// Created by jae on 14/04/23.
//

#ifndef VK_FRAMEWORK_DEPTH_UTIL_H
#define VK_FRAMEWORK_DEPTH_UTIL_H

#include <vk_loader/vk_loader.h>
#include <common/lily_structs.h>

VkFormat H_findSupportedFormat(VkPhysicalDevice ,
                               VkDevice ,
                               VkPhysicalDeviceMemoryProperties ,
                               VkCommandBuffer ,
                               VkExtent3D );

VkFormat H_findSupportedFormat(VkPhysicalDevice& ,
                               const std::vector<VkFormat>& ,
                               VkImageTiling ,
                               VkFormatFeatureFlags );

VkFormat H_findDepthFormat(VkPhysicalDevice&);

bool hasStencilComponent(VkFormat);

void H_createDepthResources(VkPhysicalDevice,
                            VkDevice,
                            VkPhysicalDeviceMemoryProperties,
                            VkCommandBuffer,
                            VkQueue,
                            VkExtent3D,
                            ImageInfo&);

#endif //VK_FRAMEWORK_DEPTH_UTIL_H
