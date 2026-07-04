#pragma once

#include <HUH/RHI/shader.h>

#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

#include <HUH/types.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanShader : public Shader {
public:
    friend class VulkanDevice;
    friend class VulkanPipeline;

    bool Init(Stage stage, const std::string& entryFunctionName) override;
    void Destroy() override;
    static VkShaderStageFlags ConvertStage(Stage stage);

protected:
    explicit VulkanShader(VulkanDevice* device, void* byteCode, Uint64 size)
        : Shader(byteCode, size),
          m_device(device) {}
    ~VulkanShader() override;
    static VkShaderStageFlagBits ConvertStageBits(Stage stage);

    VulkanDevice* m_device;
    VkShaderModule m_shaderModule = nullptr;
    VkPipelineShaderStageCreateInfo m_shaderStageInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    };
};

HUH_ENUM_BIT_OPERATORS(VkShaderStageFlagBits);
}// namespace HUH::RHI
