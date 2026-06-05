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
    void AddShader(Shader* shader) override;
    Buffer* CreateBuffer(Buffer::Type type, Uint64 Size) override;
    Buffer* CreateBuffer(Buffer::Type type, Uint64 Size, Uint64 Binding) override;
    HUH::RHI::Image* CreateImage(Image::Type type, const HUH::Vector2u32& size) override;
    HUH::RHI::Image* CreateImage(Image::Type type, const HUH::Vector2u32& size, Uint64 Binding) override;

    // ReSharper disable once CppMemberFunctionMayBeConst CppNonExplicitConversionOperator
    operator VkPipeline() { return m_pipeline; }
    // TODO move to dynamic rhi
    static VkFormat ConvertToFormat(const VertexFactory::Descriptor& descriptor);
    static VkVertexInputRate ConvertToVertexInputRate(const VertexFactory::InputRate& rate);
    static VkPipelineStageFlags ConvertToPipelineStage(const Pipeline::Stages& stages);
    static VkDescriptorType ConvertDescriptorType(const Pipeline::DescriptorTypes& descriptor);

protected:
    VulkanPipeline(VulkanDevice* device) : m_device(device) {}
    ~VulkanPipeline() override;

    struct DescriptorSet {
        VkDescriptorSet set = nullptr;
        std::vector<bool> bound;
    };

    void CreateDescriptorSet();

    VulkanDevice* m_device;
    std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayoutBindings;
    VkDescriptorSetLayout m_descriptorSetLayout = nullptr;
    VkPipelineLayout m_layout = nullptr;
    VkPipeline m_pipeline = nullptr;
    VkDescriptorPool m_descriptorPool = nullptr;
    std::vector<DescriptorSet> m_descriptorSets;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};
}// namespace HUH::RHI