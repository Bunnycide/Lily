
#ifndef VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H
#define VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H

#include <common/lily_enums.h>
#include <common/lily_structs.h>

void H_createVulkanInstance(VkInstance&,
                            ContextType&);

void H_findAllPhysicalDevices(VkInstance& ,
                              std::vector<VkPhysicalDevice>&);
void H_pickSuitablePhysicalDevice(VkPhysicalDevice& ,
                                  std::vector<VkPhysicalDevice>&);

void H_createLogicalDevice(VkDevice&,
                           VkPhysicalDevice&,
                           QueueInfo& ,
                           QueueInfo& ,
                           QueueInfo& );

void H_retrieveDeviceQueues(VkDevice &device,
                            QueueInfo &graphicsQueueInfo,
                            QueueInfo &transferQueueInfo,
                            QueueInfo &computeQueueInfo);

#endif //#ifndef VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H
#define VK_FRAMEWORK_VK_VALIDATION_LAYER_UTILS_H