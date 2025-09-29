#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
#include <HUH/logging.h>
#include <iostream>
#include <vector>
#include <HUH/types.h>
#include <HUH/RHI//vulkan/vulkan_defines.h>
#include <HUH/string_operations.h>
#include <HUH/VulkanHelpers/string_converters.h>

namespace HUH::RHI {
bool VulkanDynamicRHI::Init() {
    if (!HUH::LoadVulkan()) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Vulkan Load Failed")
        return false;
    }

    Uint32 extensionCount;
    HUH::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    HUH::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

#if HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Available Instance extensions:")
    for (size_t i = 0; i < availableExtensions.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "{}.\t{}", i + 1, availableExtensions[i].extensionName)
    }
#endif

    const std::string requiredExtensionsString = HUH_REQUIRED_INSTANCE_EXTENSIONS;
    std::vector<std::string> requiredExtensionsStrings = HUH::Split(requiredExtensionsString, ";");
    std::vector<const char*> requiredExtensions;

    for (auto& extension : requiredExtensionsStrings) {
        auto found_extension = std::find_if(availableExtensions.begin(), availableExtensions.end(),
                                            [extension](const VkExtensionProperties& properties) {
                                                if (std::string(properties.extensionName) == extension) {
                                                    return true;
                                                }
                                                return false;
                                            });
        if (found_extension == availableExtensions.end()) {
            HUH_LOG(LogVulkanRHI, Logging::Level::Warning, "Required instance extension is not available skipping: {}",
                    extension)
            continue;
        }
        requiredExtensions.push_back(extension.c_str());
    }

    Uint32 layerCount;
    HUH::vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    HUH::vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    const std::string requiredLayersString = HUH_REQUIRED_INSTANCE_LAYERS;
    std::vector<std::string> requiredLayersStrings = HUH::Split(requiredLayersString, ";");
    std::vector<const char*> requiredLayers;

#if HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Available Layers:")
    for (size_t i = 0; i < availableLayers.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "{}.\t{}", i + 1, availableLayers[i].layerName)
    }
#endif

    for (auto& layer : requiredLayersStrings) {
        auto found_extension =
            std::find_if(availableLayers.begin(), availableLayers.end(), [layer](const VkLayerProperties& properties) {
                if (std::string(properties.layerName) == layer) {
                    return true;
                }
                return false;
            });
        if (found_extension == availableLayers.end()) {
            HUH_LOG(LogVulkanRHI, Logging::Level::Warning, "Required instance extension is not available skipping: {}",
                    layer)
            continue;
        }
        requiredLayers.push_back(layer.c_str());
    }

    VkApplicationInfo appInfo{.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                              .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                              .apiVersion = VK_API_VERSION_1_4};

    const VkInstanceCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(requiredLayers.size()),
        .ppEnabledLayerNames = requiredLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
        .ppEnabledExtensionNames = requiredExtensions.data(),
    };
    auto error = HUH::vkCreateInstance(&createInfo, nullptr, &m_instance);
    if (error != VK_SUCCESS) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Vulkan Instance creation failed: {}", HUH::ToString(error))
        return false;
    }

    HUH::LoadVulkanInstance(m_instance);

    HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Vulkan Rhi Inited Successfully")
    return true;
}
Device* VulkanDynamicRHI::GetDevice(size_t index) {
    return nullptr;
}

std::vector<Device*> VulkanDynamicRHI::GetDevices() {
    return {};
}

void VulkanDynamicRHI::Destroy() {
    if (m_instance != VK_NULL_HANDLE) {
        HUH::vkDestroyInstance(m_instance, nullptr);
    }
    delete this;
}
extern "C" HUH_API HUH::RHI::DynamicRHI* DynamicRHICreate() {
    return (HUH::RHI::DynamicRHI*)new VulkanDynamicRHI();
}

}// namespace HUH::RHI
