//
// Created by jae on 19/04/23.
//

#ifndef VK_FRAMEWORK_TEXTURE2D_H
#define VK_FRAMEWORK_TEXTURE2D_H

#include <common/lily_structs.h>
#include <Device/Device.h>

bool H_LoadImageDataFromFile(const char * filePath,
                             ImageInfo& imageInfo);

class Texture2D {
public:
    ImageInfo texture;
    VkSampler  sampler;

    Texture2D(Device, const char* );
    void updateDescriptorInfo(VkDevice logicalDevice,
                              VkDescriptorSet descriptorSet,
                              uint32_t targetDescriptorBinding,
                              uint32_t targetArrayElement);
};


#endif //VK_FRAMEWORK_TEXTURE2D_H
