#pragma once

#include <HUH/RHI/pipeline.h>
#include <HUH/RHI/vertex_factory.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class VulkanPipeline : public Pipeline {
public:
    friend class VulkanDevice;
    friend class VulkanCommandPool;
    friend class VulkanImage;
    bool Init(Initializer&& initializer) override;
    void Destroy() override;
    void AddShader(class Shader* shader) override;

    // ReSharper disable once CppMemberFunctionMayBeConst CppNonExplicitConversionOperator
    operator VkPipeline() { return m_pipeline; }
    // TODO move to dynamic rhi
    static VkFormat ConvertToFormat(const VertexFactory::Descriptor& descriptor);
    static VkPipelineStageFlags ConvertToPipelineStage(const Pipeline::Stages& stages);
    static VkDescriptorType ConvertDescriptorType(const Pipeline::DescriptorTypes& descriptor);

protected:
    VulkanPipeline(VulkanDevice* device) : m_device(device) {}
    ~VulkanPipeline() override;

    VulkanDevice* m_device;
    VkDescriptorSetLayout m_descriptorSetLayout = nullptr;
    VkPipelineLayout m_layout = nullptr;
    VkPipeline m_pipeline = nullptr;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};
}// namespace HUH::RHI