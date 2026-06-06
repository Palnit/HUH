#include <HUH/RHI/vulkan/pipeline.h>

#include "HUH/RHI/vulkan/dynamic_rhi.h"

#include <HUH/RHI/vulkan/Types/buffer.h>
#include <HUH/RHI/vulkan/Types/image.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/render_pass.h>
#include <HUH/RHI/vulkan/shader.h>

#include <HUH/RHI/vertex_factory.h>

#include <HUH/enum_helper.h>
#include <HUH/types.h>

namespace HUH::RHI {

constexpr Uint32 DefaultMaxSets = 16;

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
VkPolygonMode VulkanPipeline::ConvertToPolygonMode(const VertexFactory::PolygonMode& tri) {
    switch (tri) {
        case VertexFactory::PolygonMode::Point:
            return VK_POLYGON_MODE_POINT;
        case VertexFactory::PolygonMode::Line:
            return VK_POLYGON_MODE_LINE;
        case VertexFactory::PolygonMode::Fill:
            return VK_POLYGON_MODE_FILL;
        default:
            return VK_POLYGON_MODE_FILL;
    }
}

VkVertexInputRate VulkanPipeline::ConvertToVertexInputRate(const VertexFactory::InputRate& rate) {
    switch (rate) {
        case VertexFactory::InputRate::Vertex:
            return VK_VERTEX_INPUT_RATE_VERTEX;
        case VertexFactory::InputRate::Instance:
            return VK_VERTEX_INPUT_RATE_INSTANCE;
        default:
            return VK_VERTEX_INPUT_RATE_VERTEX;
    }
}
VkPipelineStageFlags VulkanPipeline::ConvertToPipelineStage(const Pipeline::Stages& stages) {
    VkPipelineStageFlags result = 0;
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::TopOfPipe)) {
        result |= VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::DrawIndirect)) {
        result |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::VertexInput)) {
        result |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::VertexShader)) {
        result |= VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::TessellationControlShader)) {
        result |= VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::TessellationEvaluationShader)) {
        result |= VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::GeometryShader)) {
        result |= VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::FragmentShader)) {
        result |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::EarlyFragmentTests)) {
        result |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::LateFragmentTests)) {
        result |= VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::ColorAttachmentOutput)) {
        result |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::ComputeShader)) {
        result |= VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::Transfer)) {
        result |= VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::BottomOfPipe)) {
        result |= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::Host)) {
        result |= VK_PIPELINE_STAGE_HOST_BIT;
    }
    if (HUH::CheckFlag(stages, HUH::RHI::Pipeline::Stages::AllGraphics)) {
        result |= VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
    }
    return result;
}

VkDescriptorType VulkanPipeline::ConvertDescriptorType(const Pipeline::DescriptorTypes& descriptor) {
    switch (descriptor) {
        case DescriptorTypes::Uniform:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case DescriptorTypes::Sampler:
            return VK_DESCRIPTOR_TYPE_SAMPLER;
        case DescriptorTypes::ImageSampler:
            return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        default:
            return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }
}
void VulkanPipeline::CreateDescriptorSet() {
    VkDescriptorSetAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = m_descriptorPool,
        // This should match the number of command buffers but for now it can stay
        // This should be created based on which command buffer i am bound too and each should have its own sets.
        .descriptorSetCount = 1,
        .pSetLayouts = &m_descriptorSetLayout,
    };
    m_descriptorSets.emplace_back(nullptr, std::vector<bool>(m_descriptorSetLayoutBindings.size(), false));
    if (auto err = HUH::vkAllocateDescriptorSets(*m_device, &allocInfo, &m_descriptorSets.back().set);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Failed to create descriptor set: {}")
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
    Uint32 attribute = 0;
    for (auto stream : initializer.vertexFactory.m_streams) {
        bindingDescription.emplace_back();
        bindingDescription.back().binding = binding;
        bindingDescription.back().inputRate = ConvertToVertexInputRate(stream.Rate);
        bindingDescription.back().stride = stream.Stride;
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
        .polygonMode = ConvertToPolygonMode(initializer.vertexFactory.m_polygonMode),
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

    for (auto descriptor : initializer.descriptorTypes) {
        VkDescriptorSetLayoutBinding pipelineLayoutBinding{
            .binding = static_cast<Uint32>(m_descriptorSetLayoutBindings.size()),
            .descriptorType = ConvertDescriptorType(descriptor.type),
            .descriptorCount = descriptor.count,
            .stageFlags = VulkanShader::ConvertStage(descriptor.stage),
            .pImmutableSamplers = nullptr,
        };
        m_descriptorSetLayoutBindings.push_back(pipelineLayoutBinding);
    }

    if (!m_descriptorSetLayoutBindings.empty()) {
        VkDescriptorSetLayoutCreateInfo layoutInfo{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = static_cast<Uint32>(m_descriptorSetLayoutBindings.size()),
            .pBindings = m_descriptorSetLayoutBindings.data(),
        };
        if (auto err = HUH::vkCreateDescriptorSetLayout(*m_device, &layoutInfo, nullptr, &m_descriptorSetLayout);
            err != VK_SUCCESS) {
            HUH_ELOG(LogVulkanRHI, "Error Creating Descriptor Set Layout {}", err)
        }
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = static_cast<Uint32>(m_descriptorSetLayout == nullptr ? 0 : 1),
        .pSetLayouts = m_descriptorSetLayout == nullptr ? nullptr : &m_descriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = nullptr,
    };

    if (auto err = HUH::vkCreatePipelineLayout(m_device->m_device, &pipelineLayoutInfo, nullptr, &m_layout);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error during Pipeline Layout creation: {}", err);
        return false;
    }

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    if (initializer.depthTest) {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;// Optional
        depthStencil.maxDepthBounds = 1.0f;// Optional
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};// Optional
        depthStencil.back = {}; // Optional
    }

    auto vk_renderPass = dynamic_cast<VulkanRenderPass*>(initializer.renderPass);
    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = m_shaderStages.data(),
        .pVertexInputState = &vertexInputCreateInfo,
        .pInputAssemblyState = &inputAssemblyCreateInfo,
        .pViewportState = &viewportStateCreateInfo,
        .pRasterizationState = &rasterizationStateCreateInfo,
        .pMultisampleState = &multisampleStateCreateInfo,
        .pDepthStencilState = initializer.depthTest ? &depthStencil : nullptr,
        .pColorBlendState = &colorBlending,
        .pDynamicState = &dynamicStateCreateInfo,
        .layout = m_layout,
        .renderPass = *vk_renderPass,
        // TODO renderpass subpass index ?
        .subpass = 0,
        .basePipelineHandle = nullptr,// Optional
        .basePipelineIndex = -1,      // Optional
    };

    if (auto err = vkCreateGraphicsPipelines(m_device->m_device, nullptr, 1, &pipelineInfo, nullptr, &m_pipeline);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error creating graphics pipeline: {}", err);
        return false;
    }
    VkDescriptorPoolSize uniformPoolSize[2]{{
                                                .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                .descriptorCount = 2 * HUH::RHI::DefaultMaxSets,
                                            },
                                            {
                                                .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                .descriptorCount = 2 * HUH::RHI::DefaultMaxSets,
                                            }};
    VkDescriptorPoolCreateInfo poolInfo{
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = HUH::RHI::DefaultMaxSets,
        .poolSizeCount = 2,
        .pPoolSizes = &uniformPoolSize[0],
    };
    if (auto err = vkCreateDescriptorPool(*m_device, &poolInfo, nullptr, &m_descriptorPool); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error creating descriptor pool: {}", err);
        return false;
    }

    HUH_ILOG(LogVulkanRHI, "Pipeline Creation Successful")

    return true;
}

void VulkanPipeline::Destroy() {
    Pipeline::Destroy();
    HUH::vkDestroyDescriptorPool(*m_device, m_descriptorPool, nullptr);
    HUH::vkDestroyPipeline(m_device->m_device, m_pipeline, nullptr);
    HUH::vkDestroyPipelineLayout(m_device->m_device, m_layout, nullptr);
    HUH::vkDestroyDescriptorSetLayout(m_device->m_device, m_descriptorSetLayout, nullptr);
}

void VulkanPipeline::AddShader(class Shader* shader) {
    auto vk_shader = dynamic_cast<VulkanShader*>(shader);
    m_shaderStages.push_back(vk_shader->m_shaderStageInfo);
}

Buffer* VulkanPipeline::CreateBuffer(Buffer::Type type, Uint64 Size) {
    VkBuffer buffer;
    VkBufferCreateInfo bufferCreateInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                        .pNext = nullptr,
                                        .size = Size,
                                        .usage = VulkanBuffer::ConvertBufferType(type),
                                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE};
    if (auto err = HUH::vkCreateBuffer(*m_device, &bufferCreateInfo, nullptr, &buffer); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan buffer creation failed: {}", HUH::ToString(err))
    }
    m_createdBuffers.push_back(new VulkanBuffer(Size, buffer, m_device));
    return m_createdBuffers.back();
}

Buffer* VulkanPipeline::CreateBuffer(Buffer::Type type, Uint64 Size, Uint64 Binding) {
    if (Binding >= m_createdBuffers.size()) {
        HUH_ELOG(LogVulkanRHI, "Requesting a buffer for binding greater than the specified Bindings")
        return nullptr;
    }
    // TODO error on non uniform type

    VkBuffer buffer;
    VkBufferCreateInfo bufferCreateInfo{.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
                                        .pNext = nullptr,
                                        .size = Size,
                                        .usage = VulkanBuffer::ConvertBufferType(type),
                                        .sharingMode = VK_SHARING_MODE_EXCLUSIVE};
    if (auto err = HUH::vkCreateBuffer(*m_device, &bufferCreateInfo, nullptr, &buffer); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan buffer creation failed: {}", HUH::ToString(err))
        return nullptr;
    }
    auto vk_buffer = new VulkanBuffer(Size, buffer, m_device);
    m_createdBuffers.push_back(vk_buffer);
    vk_buffer->m_uniformBuffer = true;
    vk_buffer->m_binding = Binding;

    if (m_descriptorSets.empty()) {
        CreateDescriptorSet();
    }
    bool found = false;
    for (auto& [set, bound] : m_descriptorSets) {
        if (!bound[Binding]) {
            vk_buffer->m_descriptorSet = set;
            bound[Binding] = true;
            found = true;
            break;
        }
    }
    if (!found) {
        CreateDescriptorSet();
    }
    m_descriptorSets.back().bound[Binding] = true;
    vk_buffer->m_descriptorSet = m_descriptorSets.back().set;
    return m_createdBuffers.back();
}

HUH::RHI::Image* VulkanPipeline::CreateImage(Image::Type type, const HUH::Vector2u32& size) {
    VkImage vk_tempImage;
    VkImageCreateInfo imageCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        // TODO FIX THIS
        .format = VulkanDynamicRHI::ConvertFormat(type == Image::Type::DeptStencil ? Format::D32_FLOAT_S8_UINT
                                                                                   : Format::R8G8B8A8_SRGB),
        .extent = {.width = size.Width(), .height = size.Height(), .depth = 1},
        .mipLevels = 1,
        .arrayLayers = 1,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = VulkanImage::ConvertImageUsage(type),
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};

    if (auto err = HUH::vkCreateImage(*m_device, &imageCreateInfo, nullptr, &vk_tempImage); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan buffer creation failed: {}", HUH::ToString(err))
        return nullptr;
    }
    auto vk_image = new VulkanImage(m_device, vk_tempImage, true);
    m_createdImages.push_back(vk_image);
    return m_createdImages.back();
}

HUH::RHI::Image* VulkanPipeline::CreateImage(Image::Type type, const HUH::Vector2u32& size, Uint64 Binding) {

    VkImage vk_tempImage;
    VkImageCreateInfo imageCreateInfo = {.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                                         .imageType = VK_IMAGE_TYPE_2D,
                                         .format = VulkanDynamicRHI::ConvertFormat(Format::R8G8B8A8_SRGB),
                                         .extent = {.width = size.Width(), .height = size.Height(), .depth = 1},
                                         .mipLevels = 1,
                                         .arrayLayers = 1,
                                         .samples = VK_SAMPLE_COUNT_1_BIT,
                                         .tiling = VK_IMAGE_TILING_OPTIMAL,
                                         .usage = VulkanImage::ConvertImageUsage(type),
                                         .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                                         .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED};

    if (auto err = HUH::vkCreateImage(*m_device, &imageCreateInfo, nullptr, &vk_tempImage); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Vulkan buffer creation failed: {}", HUH::ToString(err))
        return nullptr;
    }
    auto vk_image = new VulkanImage(m_device, vk_tempImage, true);
    m_createdImages.push_back(vk_image);
    vk_image->m_binding = Binding;

    if (m_descriptorSets.empty()) {
        CreateDescriptorSet();
    }
    bool found = false;
    for (auto& [set, bound] : m_descriptorSets) {
        if (!bound[Binding]) {
            vk_image->m_descriptorSet = set;
            bound[Binding] = true;
            found = true;
            break;
        }
    }
    if (!found) {
        CreateDescriptorSet();
    }
    m_descriptorSets.back().bound[Binding] = true;
    vk_image->m_descriptorSet = m_descriptorSets.back().set;
    return m_createdImages.back();
}
bool VulkanPipeline::DestroyImage(HUH::RHI::Image* image) {
    auto it = std::remove(m_createdImages.begin(), m_createdImages.end(), image);
    if (it != m_createdImages.end()) {
        Pipeline::DestroyImage(image);
    }
    m_createdImages.erase(it, m_createdImages.end());
    return true;
}

bool VulkanPipeline::DestroyBuffer(HUH::RHI::Buffer* buffer) {
    auto it = std::remove(m_createdBuffers.begin(), m_createdBuffers.end(), buffer);
    if (it != m_createdBuffers.end()) {
        Pipeline::DestroyBuffer(buffer);
    }
    m_createdBuffers.erase(it, m_createdBuffers.end());
    return true;
}

VulkanPipeline::~VulkanPipeline() {
    HUH_ILOG(LogVulkanRHI, "VulkanPipeline Destroyed")
}

}// namespace HUH::RHI