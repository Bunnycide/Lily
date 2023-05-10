//
// Created by jae on 05/04/23.
//

#include <vk_loader/vk_loader.h>
#include <Renderer/utils/vk_render_X_utils.h>
#include <array>
#include <common/lily_macros.h>
#include <common/lily_structs.h>
#include "Window/plat_window.h"

void H_createRenderPass(VkDevice& logicalDevice,
                        VkFormat colorAttachmentFormat,
                        VkFormat depthAttachmentFormat,
                        VkRenderPass& pRenderPass){

    VkAttachmentDescription colorPassAttachment{};
    colorPassAttachment.format          = colorAttachmentFormat;
    colorPassAttachment.samples         = VK_SAMPLE_COUNT_1_BIT;
    colorPassAttachment.loadOp          = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorPassAttachment.storeOp         = VK_ATTACHMENT_STORE_OP_STORE;
    colorPassAttachment.stencilLoadOp   = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorPassAttachment.stencilStoreOp  = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorPassAttachment.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
    colorPassAttachment.finalLayout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription depthPassAttachment;
    /* VkAttachmentDescriptionFlags */   depthPassAttachment.flags               = 0;
    /* VkFormat                     */   depthPassAttachment.format              = depthAttachmentFormat;
    /* VkSampleCountFlagBits        */   depthPassAttachment.samples             = VK_SAMPLE_COUNT_1_BIT;
    /* VkAttachmentLoadOp           */   depthPassAttachment.loadOp              = VK_ATTACHMENT_LOAD_OP_CLEAR;
    /* VkAttachmentStoreOp          */   depthPassAttachment.storeOp             = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    /* VkAttachmentLoadOp           */   depthPassAttachment.stencilLoadOp       = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    /* VkAttachmentStoreOp          */   depthPassAttachment.stencilStoreOp      = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    /* VkImageLayout                */   depthPassAttachment.initialLayout       = VK_IMAGE_LAYOUT_UNDEFINED;
    /* VkImageLayout                */   depthPassAttachment.finalLayout         = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference{};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array<VkAttachmentDescription, 2> attachments = {colorPassAttachment, depthPassAttachment};

    VkSubpassDescription subpassDescription{};

    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

    VkSubpassDependency subpassDependency{};

    /* uint32_t             */   subpassDependency.srcSubpass      = VK_SUBPASS_EXTERNAL;
    /* uint32_t             */   subpassDependency.dstSubpass      = 0;
    /* VkPipelineStageFlags */   subpassDependency.srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    /* VkPipelineStageFlags */   subpassDependency.dstStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    /* VkAccessFlags        */   subpassDependency.srcAccessMask   = 0;
    /* VkAccessFlags        */   subpassDependency.dstAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderPassCreateInfo.attachmentCount    = static_cast<uint32_t>(attachments.size()) ;
    renderPassCreateInfo.pAttachments       = attachments.data();
    renderPassCreateInfo.subpassCount       = 1;
    renderPassCreateInfo.pSubpasses         = &subpassDescription;
    renderPassCreateInfo.dependencyCount    = 1;
    renderPassCreateInfo.pDependencies      = &subpassDependency;

    VK_CHECK_RESULT(vkCreateRenderPass(logicalDevice, &renderPassCreateInfo, nullptr, &pRenderPass))
}

void H_createPipelineLayout(VkDevice& logicalDevice,
                            VkPipelineLayout& pipelineLayout,
                            std::vector<VkDescriptorSetLayout> &layouts){

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};

    pipelineLayoutCreateInfo.pNext                     = nullptr;
    pipelineLayoutCreateInfo.flags                     = 0;
    pipelineLayoutCreateInfo.setLayoutCount            = static_cast<uint32_t>(layouts.size());
    pipelineLayoutCreateInfo.pSetLayouts               = layouts.data();
    pipelineLayoutCreateInfo.pushConstantRangeCount    = 0;
    pipelineLayoutCreateInfo.pPushConstantRanges       = nullptr;

    VK_CHECK_RESULT(vkCreatePipelineLayout(logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout))
}

void H_createRenderPipeline(VkDevice& logicalDevice,
                            Window                      window,
                            VertexInfo                  vertexInfo,
                            std::vector<ShaderInfo>     shaderInfos,
                            VkRenderPass&               renderPass,
                            VkPipelineLayout&           pipelineLayout,
                            VkPipeline&                 pipeline
                            ){

    VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &vertexInfo.binding_description,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInfo.attribute_descriptions.size()),
            .pVertexAttributeDescriptions = vertexInfo.attribute_descriptions.data()
    };

    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

    for(auto& shaderInfo : shaderInfos){
        shaderStages.push_back(VkPipelineShaderStageCreateInfo{
/* VkStructureType                  */ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
/* const void*                      */ .pNext = nullptr,
/* VkPipelineShaderStageCreateFlags */ .flags = 0,
/* VkShaderStageFlagBits            */ .stage = static_cast<VkShaderStageFlagBits>(shaderInfo.type),
/* VkShaderModule                   */ .module = shaderInfo.shaderModule,
/* const char*                      */ .pName = "main",
/* const VkSpecializationInfo*      */ .pSpecializationInfo = nullptr,
        });
    }

    VkViewport viewport {
        .x = 0,
        .y = 0,
        .width = static_cast<float>(window.width),
        .height = static_cast<float>(window.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissorRect {
        .offset = {
                0,0
        },
        .extent = {
                static_cast<uint32_t>(window.width),
                static_cast<uint32_t>(window.height)
        }
    };

    VkPipelineViewportStateCreateInfo viewportStateCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};

    /* const void*                        */   viewportStateCreateInfo.pNext         = nullptr;
    /* VkPipelineViewportStateCreateFlags */   viewportStateCreateInfo.flags         = 0;
    /* uint32_t                           */   viewportStateCreateInfo.viewportCount = 1;
    /* const VkViewport*                  */   viewportStateCreateInfo.pViewports    = &viewport;
    /* uint32_t                           */   viewportStateCreateInfo.scissorCount  = 1;
    /* const VkRect2D*                    */   viewportStateCreateInfo.pScissors     = &scissorRect;

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

    /* const void*                             */   rasterizationStateCreateInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    /* VkPipelineRasterizationStateCreateFlags */   rasterizationStateCreateInfo.pNext                      = nullptr;
    /* VkBool32                                */   rasterizationStateCreateInfo.flags                      = 0;
    /* VkBool32                                */   rasterizationStateCreateInfo.depthClampEnable           = VK_FALSE;
    /* VkPolygonMode                           */   rasterizationStateCreateInfo.rasterizerDiscardEnable    = VK_FALSE;
    /* VkCullModeFlags                         */   rasterizationStateCreateInfo.polygonMode                = VK_POLYGON_MODE_FILL;
    /* VkFrontFace                             */   rasterizationStateCreateInfo.cullMode                   = VK_CULL_MODE_FRONT_BIT;
    /* VkBool32                                */   rasterizationStateCreateInfo.frontFace                  = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasEnable            = VK_FALSE;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasConstantFactor    = 0.0f;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasClamp             = 0.0f;
    /* float                                   */   rasterizationStateCreateInfo.depthBiasSlopeFactor       = 0.0f  ;
    /* float                                   */   rasterizationStateCreateInfo.lineWidth                  = 1.0f;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{
            /* VkBool32              */   .blendEnable         = VK_TRUE,
            /* VkBlendFactor         */   .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            /* VkBlendFactor         */   .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            /* VkBlendOp             */   .colorBlendOp        = VK_BLEND_OP_ADD,
            /* VkBlendFactor         */   .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            /* VkBlendFactor         */   .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            /* VkBlendOp             */   .alphaBlendOp        = VK_BLEND_OP_ADD,
            /* VkColorComponentFlags */   .colorWriteMask      =    VK_COLOR_COMPONENT_R_BIT |
                                                                    VK_COLOR_COMPONENT_G_BIT |
                                                                    VK_COLOR_COMPONENT_B_BIT |
                                                                    VK_COLOR_COMPONENT_A_BIT,
    };
                                                            
    VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};

    /* const void*                                */   colorBlendStateCreateInfo.pNext             = nullptr;
    /* VkPipelineColorBlendStateCreateFlags       */   colorBlendStateCreateInfo.flags             = 0;
    /* VkBool32                                    */  colorBlendStateCreateInfo.logicOpEnable     = VK_FALSE,
    /* VkLogicOp                                   */  colorBlendStateCreateInfo.logicOp           = VK_LOGIC_OP_COPY,
    /* uint32_t                                   */   colorBlendStateCreateInfo.attachmentCount   = 1;
    /* const VkPipelineColorBlendAttachmentState* */   colorBlendStateCreateInfo.pAttachments      = &colorBlendAttachment;


    colorBlendStateCreateInfo.blendConstants[0] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[1] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[2] = 0.0f;
    colorBlendStateCreateInfo.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType              = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount  = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates     = dynamicStates.data();

    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo{
            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext                 = nullptr,
            .flags                 = 0,
            .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable   = VK_FALSE,
            .minSampleShading      = 1.0f,
            .pSampleMask           = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable      = VK_FALSE,
    };

    VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo
            {VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};

    depthStencilStateCreateInfo.pNext                   = nullptr;
    depthStencilStateCreateInfo.flags                   = 0;
    depthStencilStateCreateInfo.depthTestEnable         = VK_TRUE;
    depthStencilStateCreateInfo.depthWriteEnable        = VK_TRUE;
    depthStencilStateCreateInfo.depthCompareOp          = VK_COMPARE_OP_LESS;
    depthStencilStateCreateInfo.depthBoundsTestEnable   = VK_FALSE;
    depthStencilStateCreateInfo.stencilTestEnable       = VK_FALSE;
    depthStencilStateCreateInfo.front                   = {};
    depthStencilStateCreateInfo.back                    = {};
    depthStencilStateCreateInfo.minDepthBounds          = 0.0f;
    depthStencilStateCreateInfo.maxDepthBounds          = 1.0f;

    VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
    };

    pipelineInputAssemblyStateCreateInfo.pNext                  = nullptr;
    pipelineInputAssemblyStateCreateInfo.flags                  = 0;
    pipelineInputAssemblyStateCreateInfo.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

    VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    /* const void*                                    */  graphicsPipelineCreateInfo.pNext              = nullptr;
    /* VkPipelineCreateFlags                          */  graphicsPipelineCreateInfo.flags              = 0;
    /* uint32_t                                       */  graphicsPipelineCreateInfo.stageCount         = static_cast<uint32_t>(shaderStages.size());
    /* const VkPipelineShaderStageCreateInfo*         */  graphicsPipelineCreateInfo.pStages            = shaderStages.data();
    /* const VkPipelineVertexInputStateCreateInfo*    */  graphicsPipelineCreateInfo.pVertexInputState  = &vertexInputStateCreateInfo;
    /* const VkPipelineInputAssemblyStateCreateInfo*  */  graphicsPipelineCreateInfo.pInputAssemblyState= &pipelineInputAssemblyStateCreateInfo;
    /* const VkPipelineTessellationStateCreateInfo*   */  graphicsPipelineCreateInfo.pTessellationState = nullptr;
    /* const VkPipelineViewportStateCreateInfo*       */  graphicsPipelineCreateInfo.pViewportState     = &viewportStateCreateInfo;
    /* const VkPipelineRasterizationStateCreateInfo*  */  graphicsPipelineCreateInfo.pRasterizationState= &rasterizationStateCreateInfo;
    /* const VkPipelineMultisampleStateCreateInfo*    */  graphicsPipelineCreateInfo.pMultisampleState  = &multisampleStateCreateInfo;
    // TODO: Remember to create depth state (later)
    /* const VkPipelineDepthStencilStateCreateInfo*   */  graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilStateCreateInfo;
    /* const VkPipelineColorBlendStateCreateInfo*     */  graphicsPipelineCreateInfo.pColorBlendState   = &colorBlendStateCreateInfo;
    /* const VkPipelineDynamicStateCreateInfo*        */  graphicsPipelineCreateInfo.pDynamicState      = &dynamicState;
    /* VkPipelineLayout                               */  graphicsPipelineCreateInfo.layout             = pipelineLayout;
    /* VkRenderPass                                   */  graphicsPipelineCreateInfo.renderPass         = renderPass;
    /* uint32_t                                       */  graphicsPipelineCreateInfo.subpass            = 0;
    /* VkPipeline                                     */  graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    /* int32_t                                        */  graphicsPipelineCreateInfo.basePipelineIndex  = -1;

    VK_CHECK_RESULT(vkCreateGraphicsPipelines(logicalDevice,
                                              nullptr,
                                              1,
                                              &graphicsPipelineCreateInfo,
                                              nullptr,
                                              &pipeline))
}
