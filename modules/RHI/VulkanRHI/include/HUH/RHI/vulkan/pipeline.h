#pragma once
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/pipeline.h>

namespace HUH::RHI {
class VulkanPipeline : public Pipeline {
public:
    friend class VulkanDevice;
    friend class VulkanCommandPool;
    friend class VulkanImage;
    bool Init(Initializer&& initializer) override;
    void Destroy() override;
    void AddShader(class Shader* shader) override;

protected:
    VulkanPipeline(class VulkanDevice* device) : m_device(device) {}
    ~VulkanPipeline() override;
    class VulkanDevice* m_device;
    VkPipelineLayout m_layout = nullptr;
    VkPipeline m_pipeline = nullptr;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};
}// namespace HUH::RHI