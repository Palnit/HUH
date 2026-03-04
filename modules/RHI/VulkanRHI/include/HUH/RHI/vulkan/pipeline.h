#pragma once
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/pipeline.h>

namespace HUH::RHI {
class VulkanPipeline : public Pipeline {
public:
    friend class VulkanDynamicRHI;
    bool Init(class Device* device) override;
    void Destroy() override;
    void AddShader(class Shader* shader) override;

protected:
    VulkanPipeline() = default;
    ~VulkanPipeline() override = default;
    VkPipelineLayout m_layout = nullptr;
    VkPipeline m_pipeline = nullptr;
    class VulkanDevice* m_device = nullptr;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    // TODO refactor this to separate class maybe ?
    VkRenderPass m_renderPass = nullptr;
};
}// namespace HUH::RHI