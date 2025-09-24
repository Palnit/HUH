#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
#include <HUH/logging.h>
#include <iostream>
#include <vector>

namespace HUH::RHI {
bool VulkanDynamicRHI::Init() {
    if (HUH::LoadVulkan()) {
        std::cout << "Volk failed to initalize";
        return false;
    }

    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    auto err = HUH::vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (err != VK_SUCCESS) {
        std::cout << "create instance failed" << std::endl;
        return false;
    }
    HUH_LOG(LogRHI, Logging::Level::Log, "Vulkan Rhi Inited Successfully")
    return true;
}

void VulkanDynamicRHI::Destroy() {
    //    if (m_instance != VK_NULL_HANDLE) { vkDestroyInstance(m_instance, nullptr); }
    delete this;
}
extern "C" HUH_API HUH::RHI::DynamicRHI* DynamicRHICreate() { return (HUH::RHI::DynamicRHI*) new VulkanDynamicRHI(); }

}// namespace HUH::RHI
