//
// Created by jae on 13/04/23.
//

#ifndef VK_FRAMEWORK_VK_IMAGE_UTIL_H
#define VK_FRAMEWORK_VK_IMAGE_UTIL_H

#include <vk_loader/vk_loader.h>
#include <common/lily_structs.h>

void H_createImage(VkDevice ,
                   ImageInfo&  ,
                   VkBool32 ,
                   VkImageType ,
                   VkFormat ,
                   VkExtent3D ,
                   uint32_t ,
                   uint32_t ,
                   VkSampleCountFlagBits );

void H_allocateAndBindMemoryObjectToImage(VkDevice,
                                          ImageInfo&,
                                          VkPhysicalDeviceMemoryProperties);

void H_setupImageMemoryBarrier(std::vector<ImageTransition>& ,
                               VkQueue ,
                               VkCommandBuffer ,
                               VkPipelineStageFlags ,
                               VkPipelineStageFlags );

void H_createImageView(VkDevice ,
                       VkImage ,
                       VkFormat ,
                       VkImageViewType ,
                       VkImageAspectFlags ,
                       VkImageView& );

void H_freeImage(VkDevice,
                 ImageInfo&);

#endif //VK_FRAMEWORK_VK_IMAGE_UTIL_H
