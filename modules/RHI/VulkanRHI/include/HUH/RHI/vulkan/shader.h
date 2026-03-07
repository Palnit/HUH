#pragma once

#include <HUH/types.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/shader.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanShader : public Shader {
public:
    friend class VulkanDevice;
    friend class VulkanPipeline;

    bool Init(Stage stage, const std::string& entryFunctionName) override;
    void Destroy() override;
    static VkShaderStageFlagBits ShaderStageToVkShaderStage(Stage stage);

protected:
    explicit VulkanShader(class VulkanDevice* device, void* byteCode, Uint64 size)
        : Shader(byteCode, size),
          m_device(device) {}
    ~VulkanShader() override;

    class VulkanDevice* m_device;
    VkShaderModule m_shaderModule = nullptr;
    VkPipelineShaderStageCreateInfo m_shaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    };
};

HUH_ENUM_BIT_OPERATORS(VkShaderStageFlagBits);
}// namespace HUH::RHI
