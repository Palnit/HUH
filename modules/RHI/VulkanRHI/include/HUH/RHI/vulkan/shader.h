#pragma once

#include <HUH/types.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/shader.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanShader : public Shader {
public:
    friend class VulkanDynamicRHI;

    bool Init(Device* device, Stage stage, const std::string& entryFunctionName = "main") override;
    void Destroy() override;
    static VkShaderStageFlagBits ShaderStageToVkShaderStage(Stage stage);

protected:
    explicit VulkanShader(void* byteCode, Uint64 size) : Shader(byteCode, size) {}
    ~VulkanShader() override;

    class VulkanDevice* m_device = nullptr;
    VkShaderModule m_shaderModule = nullptr;
};

HUH_ENUM_BIT_OPERATORS(VkShaderStageFlagBits);
}// namespace HUH::RHI
