//
// Created by jae on 14/04/23.
//

#ifndef VK_FRAMEWORK_TEXTURE_UTIL_H
#define VK_FRAMEWORK_TEXTURE_UTIL_H

#include "vk_loader/vk_loader.h"
#include "common/lily_macros.h"
#include "common/lily_structs.h"
#include "Resources/utils/vk_image_util.h"

void H_CreateTexture2DSampler(VkDevice ,
                              VkSampler& );

void H_create2DImageAndView(VkDevice logicalDevice,
                            VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                            VkFormat format,
                            VkExtent2D size,
                            uint32_t numMipmaps,
                            uint32_t numLayers,
                            ImageInfo&);

#endif //VK_FRAMEWORK_TEXTURE_UTIL_H
