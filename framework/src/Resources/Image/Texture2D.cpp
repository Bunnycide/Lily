//
// Created by jae on 19/04/23.
//

#include <Resources/Image/Texture2D.h>
#include <Resources/stb_image.h>
#include <string>
#include <Resources/Image/texture_util.h>
#include <Resources/utils/vk_buffer_util.h>
#include <cstring>
#include <Resources/utils/vk_image_util.h>
#include <Commands/utils/vk_command_X_utils.h>
#include <Resources/asset_utils.h>

bool H_LoadImageDataFromFile(const char * filePath,
                                      ImageInfo& imageInfo) {

    int imgWidth, imgHeight, n;

    imageInfo.imageData = stbi_load( filePath, &imgWidth, &imgHeight, &n, 4);

    imageInfo.imgHeight = imgHeight;
    imageInfo.imgWidth = imgWidth;
    imageInfo.imgSize = imgWidth * imgHeight * 4;

    if(imageInfo.imageData == nullptr){
        return false;
    }

    return true;

}

Texture2D::Texture2D( Device device,
                        const char* filePath ){

    std::string absolute_path;
    getAssetPath(filePath, absolute_path);

    H_LoadImageDataFromFile(absolute_path.c_str(), texture);

    texture.imageUsageFlags = VK_IMAGE_USAGE_SAMPLED_BIT|
                            VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    texture.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

    H_create2DImageAndView(device.logicalDevice,
                           device.physicalDeviceMemoryProperties,
                           VK_FORMAT_R8G8B8A8_SRGB,
                           {
                                static_cast<uint32_t>(texture.imgWidth),
                                static_cast<uint32_t>(texture.imgHeight)
                           },
                           1,
                           1,
                           texture);
    // Staging buffer
    BufferInfo stagingBufferInfo;
    stagingBufferInfo.bufSz = texture.imgSize;
    stagingBufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    H_createBuffer(device.logicalDevice, stagingBufferInfo);

    H_allocateAndBindMemoryObjectToBuffer(device.physicalDevice,
                                          device.logicalDevice,
                                          device.physicalDeviceMemoryProperties,
                                          stagingBufferInfo);
    VK_CHECK_RESULT(vkMapMemory(device.logicalDevice,
                                stagingBufferInfo.memoryObj,
                                0,
                                texture.imgSize,
                                0,
                                &stagingBufferInfo.memoryPointer))

    std::memcpy(stagingBufferInfo.memoryPointer,
                texture.imageData,
                texture.imgSize);

    vkUnmapMemory(device.logicalDevice,
                  stagingBufferInfo.memoryObj);

    stbi_image_free(texture.imageData);

    VkBufferImageCopy region {
            .bufferOffset      = 0,
            .bufferRowLength   = 0,
            .bufferImageHeight = 0,
            .imageSubresource  = {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .mipLevel       = 0,
                    .baseArrayLayer = 0,
                    .layerCount     = 1,
            },

            .imageOffset       = VkOffset3D{0, 0, 0},
            .imageExtent       = VkExtent3D  {
                    static_cast<uint32_t>(texture.imgWidth),
                    static_cast<uint32_t>(texture.imgHeight),
                    1
            }
    };

    std::vector<ImageTransition> image_transition;

    image_transition.push_back({
        .image              = texture.image,
        .currentAccess      = 0,
        .newAccess          = VK_ACCESS_TRANSFER_WRITE_BIT,
        .currentLayout      = VK_IMAGE_LAYOUT_UNDEFINED,
        .newLayout          = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
        .newQueueFamily     = VK_QUEUE_FAMILY_IGNORED,
        .aspect             = VK_IMAGE_ASPECT_COLOR_BIT
    });

    H_setupImageMemoryBarrier(image_transition,
                              device.graphicsQueueSet.queues[0],
                              device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                              VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT);

    H_beginCommandBufferRecording(device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                                  VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    vkCmdCopyBufferToImage(device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                           stagingBufferInfo.buffer,
                           texture.image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                           1,
                           &region);

    H_endCommandBufferRecording(device.graphicsCommands.commandPoolInfo.commandBuffers[0]);

    VkSubmitInfo submitInfo{
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &device.graphicsCommands.commandPoolInfo.commandBuffers[0]
    };

    vkQueueSubmit(device.graphicsQueueSet.queues[0],
                  1,
                  &submitInfo,
                  VK_NULL_HANDLE);

    vkQueueWaitIdle(device.graphicsQueueSet.queues[0]);

    image_transition[0] = {
            .image = texture.image,
            .currentAccess = VK_ACCESS_TRANSFER_WRITE_BIT,
            .newAccess = VK_ACCESS_SHADER_READ_BIT,
            .currentLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            .newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .currentQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .newQueueFamily = VK_QUEUE_FAMILY_IGNORED,
            .aspect = VK_IMAGE_ASPECT_COLOR_BIT
    };

    H_setupImageMemoryBarrier(image_transition,
                              device.graphicsQueueSet.queues[0],
                              device.graphicsCommands.commandPoolInfo.commandBuffers[0],
                              VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT);

    H_CreateTexture2DSampler(device.logicalDevice, sampler);
}
