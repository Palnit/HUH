#include <volk.h>

#include <HUH/RHI/vulkan/dynamic_rhi.h>
#include <iostream>
#include <vector>

namespace HUH::RHI {
bool VulkanDynamicRHI::Init() {
    auto err = volkInitialize();
    if (err != VK_SUCCESS) {
        std::cout << "Volk failed to initalize";
        return false;
    }
    {
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
        err = vkCreateInstance(&createInfo, nullptr, &m_instance);
        if (err != VK_SUCCESS) {
            std::cout << "create instance failed" << std::endl;
            return false;
        }
    }

    volkLoadInstance(m_instance);

    return true;
}

void VulkanDynamicRHI::Destroy() {
    if (m_instance != VK_NULL_HANDLE) { vkDestroyInstance(m_instance, nullptr); }
    delete this;
}
extern "C" HUH_API RHI::DynamicRHI* DynamicRHICreate() { return new VulkanDynamicRHI(); }

}// namespace HUH::RHI
