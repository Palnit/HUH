#include <HUH/RHI/vulkan/shader.h>

#include <HUH/RHI/vulkan/device.h>

#include <HUH/enum_helper.h>

namespace HUH::RHI {

bool VulkanShader::Init(Stage stage, const std::string& entryFunctionName) {
    m_stage = stage;
    VkShaderModuleCreateInfo createInfo{.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                                        .pNext = nullptr,
                                        .codeSize = m_byteCodeSize,
                                        .pCode = reinterpret_cast<uint32_t*>(m_byteCode)};
    if (auto err = HUH::vkCreateShaderModule(m_device->m_device, &createInfo, nullptr, &m_shaderModule);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Shader Creation Error: {}", err)
        return false;
    }

    m_shaderStageInfo.stage = ConvertStageBits(m_stage);
    m_shaderStageInfo.module = m_shaderModule;
    m_shaderStageInfo.pName = entryFunctionName.c_str();

    return true;
}

void VulkanShader::Destroy() {
    HUH::vkDestroyShaderModule(m_device->m_device, m_shaderModule, nullptr);
}

VkShaderStageFlags VulkanShader::ConvertStage(Stage stage) {
    auto vk_stage = static_cast<VkShaderStageFlags>(0);
    if (CheckFlag(stage, Shader::Fragment)) {
        vk_stage |= VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    if (CheckFlag(stage, Shader::Vertex)) {
        vk_stage |= VK_SHADER_STAGE_VERTEX_BIT;
    }
    if (CheckFlag(stage, Shader::Geometry)) {
        vk_stage |= VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    if (CheckFlag(stage, Shader::Compute)) {
        vk_stage |= VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return vk_stage;
}

VulkanShader::~VulkanShader() {
    HUH_ILOG(LogVulkanRHI, "VulkanShader Destroyed");
}
VkShaderStageFlagBits VulkanShader::ConvertStageBits(Stage stage) {
    auto vk_stage = static_cast<VkShaderStageFlagBits>(0);
    if (CheckFlag(stage, Shader::Fragment)) {
        vk_stage |= VK_SHADER_STAGE_FRAGMENT_BIT;
    }
    if (CheckFlag(stage, Shader::Vertex)) {
        vk_stage |= VK_SHADER_STAGE_VERTEX_BIT;
    }
    if (CheckFlag(stage, Shader::Geometry)) {
        vk_stage |= VK_SHADER_STAGE_GEOMETRY_BIT;
    }
    if (CheckFlag(stage, Shader::Compute)) {
        vk_stage |= VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return vk_stage;
}
}// namespace HUH::RHI