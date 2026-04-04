#include <HUH/RHI/vulkan/pipeline.h>
#include <HUH/RHI/vulkan/shader.h>
#include <HUH/types.h>

#include "HUH/RHI/vertex_factory.h"

#include <HUH/RHI/vertex_factory.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>

namespace HUH::RHI {

VkFormat VulkanPipeline::ConvertToFormat(const VertexFactory::Descriptor& descriptor) {
    switch (descriptor.vectorFormat) {
        case VertexFactory::VectorFormat::X:
            switch (descriptor.format) {
                case VertexFactory::Format::U8:
                    return VK_FORMAT_R8_UINT;
                case VertexFactory::Format::U16:
                    return VK_FORMAT_R16_UINT;
                case VertexFactory::Format::U32:
                    return VK_FORMAT_R32_UINT;
                case VertexFactory::Format::U64:
                    return VK_FORMAT_R64_UINT;
                case VertexFactory::Format::I8:
                    return VK_FORMAT_R8_SINT;
                case VertexFactory::Format::I16:
                    return VK_FORMAT_R16_SINT;
                case VertexFactory::Format::I32:
                    return VK_FORMAT_R32_SINT;
                case VertexFactory::Format::I64:
                    return VK_FORMAT_R64_SINT;
                case VertexFactory::Format::F32:
                    return VK_FORMAT_R32_SFLOAT;
                case VertexFactory::Format::F64:
                    return VK_FORMAT_R64_SFLOAT;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
        case VertexFactory::VectorFormat::XY:
            switch (descriptor.format) {
                case VertexFactory::Format::U8:
                    return VK_FORMAT_R8G8_UINT;
                case VertexFactory::Format::U16:
                    return VK_FORMAT_R16G16_UINT;
                case VertexFactory::Format::U32:
                    return VK_FORMAT_R32G32_UINT;
                case VertexFactory::Format::U64:
                    return VK_FORMAT_R64G64_UINT;
                case VertexFactory::Format::I8:
                    return VK_FORMAT_R8G8_SINT;
                case VertexFactory::Format::I16:
                    return VK_FORMAT_R16G16_SINT;
                case VertexFactory::Format::I32:
                    return VK_FORMAT_R32G32_SINT;
                case VertexFactory::Format::I64:
                    return VK_FORMAT_R64G64_SINT;
                case VertexFactory::Format::F32:
                    return VK_FORMAT_R32G32_SFLOAT;
                case VertexFactory::Format::F64:
                    return VK_FORMAT_R64G64_SFLOAT;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
        case VertexFactory::VectorFormat::XYZ:
            switch (descriptor.format) {
                case VertexFactory::Format::U8:
                    return VK_FORMAT_R8G8B8_UINT;
                case VertexFactory::Format::U16:
                    return VK_FORMAT_R16G16B16_UINT;
                case VertexFactory::Format::U32:
                    return VK_FORMAT_R32G32B32_UINT;
                case VertexFactory::Format::U64:
                    return VK_FORMAT_R64G64B64_UINT;
                case VertexFactory::Format::I8:
                    return VK_FORMAT_R8G8B8_SINT;
                case VertexFactory::Format::I16:
                    return VK_FORMAT_R16G16B16_SINT;
                case VertexFactory::Format::I32:
                    return VK_FORMAT_R32G32B32_SINT;
                case VertexFactory::Format::I64:
                    return VK_FORMAT_R64G64B64_SINT;
                case VertexFactory::Format::F32:
                    return VK_FORMAT_R32G32B32_SFLOAT;
                case VertexFactory::Format::F64:
                    return VK_FORMAT_R64G64B64_SFLOAT;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
        case VertexFactory::VectorFormat::XYZW:
            switch (descriptor.format) {
                case VertexFactory::Format::U8:
                    return VK_FORMAT_R8G8B8A8_UINT;
                case VertexFactory::Format::U16:
                    return VK_FORMAT_R16G16B16A16_UINT;
                case VertexFactory::Format::U32:
                    return VK_FORMAT_R32G32B32A32_UINT;
                case VertexFactory::Format::U64:
                    return VK_FORMAT_R64G64B64A64_UINT;
                case VertexFactory::Format::I8:
                    return VK_FORMAT_R8G8B8A8_SINT;
                case VertexFactory::Format::I16:
                    return VK_FORMAT_R16G16B16A16_SINT;
                case VertexFactory::Format::I32:
                    return VK_FORMAT_R32G32B32A32_SINT;
                case VertexFactory::Format::I64:
                    return VK_FORMAT_R64G64B64A64_SINT;
                case VertexFactory::Format::F32:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
                case VertexFactory::Format::F64:
                    return VK_FORMAT_R64G64B64A64_SFLOAT;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
        default:
            return VK_FORMAT_UNDEFINED;
    }
}

bool VulkanPipeline::Init(Initializer&& initializer) {
    // TODO REFACTOR THIS TO MAKE IT WORK WITHOUT ME HAVING TO DO MAGIC
    std::vector<VkDynamicState> dynamicStates{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = static_cast<Uint32>(dynamicStates.size()),
        .pDynamicStates = dynamicStates.data(),
    };
    VkPipelineViewportStateCreateInfo viewportStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .scissorCount = 1,
    };
    std::vector<VkVertexInputBindingDescription> bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

    Uint32 binding = 0;
    for (auto stream : initializer.vertexFactory.m_streams) {
        bindingDescription.emplace_back();
        bindingDescription.back().binding = binding;
        bindingDescription.back().inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescription.back().stride = stream.Stride;
        Uint32 attribute = 0;
        for (auto descriptor : stream.descriptors) {
            attributeDescriptions.emplace_back();
            attributeDescriptions.back().binding = binding;
            attributeDescriptions.back().location = attribute++;
            attributeDescriptions.back().format = ConvertToFormat(descriptor);
            attributeDescriptions.back().offset = descriptor.Offset;
        }
        binding++;
    }

    VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = static_cast<Uint32>(bindingDescription.size()),
        .pVertexBindingDescriptions = bindingDescription.data(),
        .vertexAttributeDescriptionCount = static_cast<Uint32>(attributeDescriptions.size()),
        .pVertexAttributeDescriptions = attributeDescriptions.data(),
    };

    // TODO REFACTOR TO VERTEX FACTORY
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    // TODO REFACTOR MULTI SAMPLING
    VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };
    // TODO DEPTH AND STENCILS

    // TODO Blending correctly mostly alpha blend
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;// Optional
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;            // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;// Optional
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;            // Optional

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;// Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;// Optional
    colorBlending.blendConstants[1] = 0.0f;// Optional
    colorBlending.blendConstants[2] = 0.0f;// Optional
    colorBlending.blendConstants[3] = 0.0f;// Optional

    // TODO Uniforms
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;           // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr;        // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0;   // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr;// Optional

    if (auto err = HUH::vkCreatePipelineLayout(m_device->m_device, &pipelineLayoutInfo, nullptr, &m_layout);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error during Pipeline Layout creation: {}", err);
        return false;
    }

    std::vector<VkFormat> vk_formats;
    vk_formats.reserve(initializer.formats.size());
    for (auto& format : initializer.formats) {
        vk_formats.push_back(VulkanDynamicRHI::ConvertFormat(format));
    }

    VkPipelineRenderingCreateInfo pipelineRenderingCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
        .colorAttachmentCount = static_cast<Uint32>(vk_formats.size()),
        .pColorAttachmentFormats = vk_formats.data()};

    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .pNext = &pipelineRenderingCreateInfo,
        .stageCount = 2,
        .pStages = m_shaderStages.data(),
        .pVertexInputState = &vertexInputCreateInfo,
        .pInputAssemblyState = &inputAssemblyCreateInfo,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = nullptr,// Optional
        .pColorBlendState = &colorBlending,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = m_layout,
        .renderPass = nullptr,
        .subpass = 0,
        .basePipelineHandle = nullptr,// Optional
        .basePipelineIndex = -1,      // Optional
    };

    if (auto err = vkCreateGraphicsPipelines(m_device->m_device, nullptr, 1, &pipelineInfo, nullptr, &m_pipeline);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error creating graphics pipeline: {}", err);
        return false;
    }
    HUH_ILOG(LogVulkanRHI, "Pipeline Creation Successful")

    return true;
}

void VulkanPipeline::Destroy() {
    HUH::vkDestroyPipeline(m_device->m_device, m_pipeline, nullptr);
    HUH::vkDestroyPipelineLayout(m_device->m_device, m_layout, nullptr);
}

void VulkanPipeline::AddShader(class Shader* shader) {
    auto vk_shader = dynamic_cast<VulkanShader*>(shader);
    m_shaderStages.push_back(vk_shader->m_shaderStageInfo);
}

VulkanPipeline::~VulkanPipeline() {
    HUH_ILOG(LogVulkanRHI, "VulkanPipeline Destroyed")
}

}// namespace HUH::RHI