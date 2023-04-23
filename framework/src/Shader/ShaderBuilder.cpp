//
// Created by jae on 21/04/23.
//

#include <Shader/ShaderBuilder.h>
#include <memory>
#include <Shader/utils/shader_utils.h>
#include <Shader/utils/vk_descriptor_util.h>
#include <Shader/utils/spv_reflect_util.h>

ShaderBuilder* ShaderBuilder::addShader(Device device, std::string source, VkShaderStageFlags stage) {
    Shader shader = Shader();
    VkShaderModule module;

    shader.createShader(source, stage);
    buildShaderFromSource(device.logicalDevice,
                          shader.mspv,
                          &module);

    shaderModules.push_back({module, stage});

    if(stage == VK_SHADER_STAGE_VERTEX_BIT){
        vertexInfo = generateVertexInfo(shader.mspv);
    }

    auto descData  = generateDescriptorInfo(shader.mspv);

    // First set by default (Avoid multiple sets where can)
    int currentSetNumber = 0;

    // If vector is not empty, not the first reflect run, update the setNumber to latest
    if(! allDescriptorData.empty()) currentSetNumber = (allDescriptorData.back().set_number);

    for(int i = 0; i < descData.size(); i++) {
        if( descData[i].set_number > currentSetNumber ||
            allDescriptorData.empty())
            allDescriptorData.push_back(descData[i]);
        // Set number is same try and assimilate the descriptor data
        else{
            int index = allDescriptorData.size() - 1;

            DescriptorSetLayoutData lastElement = allDescriptorData.back();

            DescriptorSetLayoutData mergeData;
            // Set "set number"
            mergeData.set_number = descData[i].set_number;

            for(auto& data : lastElement.bindings){
                mergeData.bindings.push_back(data);
            }

            for(auto& data : descData[i].bindings){
                mergeData.bindings.push_back(data);
            }

            mergeData.create_info.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            mergeData.create_info.pNext         = nullptr;
            mergeData.create_info.flags         = 0;
            mergeData.create_info.bindingCount  = mergeData.bindings.size();
            mergeData.create_info.pBindings     = mergeData.bindings.data();

            allDescriptorData[allDescriptorData.size() - 1] = mergeData;

        }
    }

    return this;
}
