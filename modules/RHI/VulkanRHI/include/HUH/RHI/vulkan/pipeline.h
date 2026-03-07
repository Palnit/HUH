#pragma once
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/pipeline.h>

namespace HUH::RHI {
class VulkanPipeline : public Pipeline {
public:
    friend class VulkanDevice;
    bool Init() override;
    void Destroy() override;
    void AddShader(class Shader* shader) override;

protected:
    VulkanPipeline(class VulkanDevice* device) : m_device(device) {}
    ~VulkanPipeline() override = default;
    class VulkanDevice* m_device;
    VkPipelineLayout m_layout = nullptr;
    VkPipeline m_pipeline = nullptr;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    // TODO refactor this to separate class maybe ?
    VkRenderPass m_renderPass = nullptr;
};
}// namespace HUH::RHI