#include <HUH/RHI/vulkan/command_buffer.h>
#include <HUH/RHI/vulkan/pipeline.h>

#include <HUH/RHI/vulkan/swapchain.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>

#include "HUH/RHI/vulkan/shader.h"

#include <vector>
#include <HUH/types.h>
#include <HUH/string_operations.h>

namespace HUH::RHI {
bool VulkanDynamicRHI::Init() {
    if (!HUH::LoadVulkan()) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Error, "Vulkan Load Failed")
        return false;
    }

    Uint32 extensionCount;
    HUH::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    HUH::vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

#if HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::DebugLog, "{} Available Instance extensions:", extensionCount)
    for (size_t i = 0; i < availableExtensions.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::DebugLog, "{}.\t{}", i + 1, availableExtensions[i].extensionName)
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
    HUH_LOG(LogVulkanRHI, Logging::Level::DebugLog, "Available Layers:")
    for (size_t i = 0; i < availableLayers.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::Level::DebugLog, "{}.\t{}", i + 1, availableLayers[i].layerName)
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
    if (const auto error = HUH::vkCreateInstance(&createInfo, nullptr, &m_instance); error != VK_SUCCESS) {
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
    Uint32 deviceCount = 0;
    HUH::vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "No Physical device with vulkan support");
        return {};
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    HUH::vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());
    for (auto device : devices) {
        m_createdDevices.push_back(new VulkanDevice(this, device));
    }

    return m_createdDevices;
}

VkFormat VulkanDynamicRHI::ConvertFormat(Format format) {
    switch (format) {
        case Format::R8G8B8A8_UNORM:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case Format::R8G8B8A8_SRGB:
            return VK_FORMAT_R8G8B8A8_SRGB;
        case Format::B8G8R8A8_UNORM:
            return VK_FORMAT_B8G8R8A8_UNORM;
        case Format::B8G8R8A8_SRGB:
            return VK_FORMAT_B8G8R8A8_SRGB;
        case Format::UNKNOWN:
            return VK_FORMAT_UNDEFINED;
        default:
            return VK_FORMAT_UNDEFINED;
    }
}

Format VulkanDynamicRHI::ConvertFormat(VkFormat format) {
    switch (format) {
        case VK_FORMAT_R8G8B8A8_UNORM:
            return Format::R8G8B8A8_UNORM;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return Format::R8G8B8A8_SRGB;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return Format::B8G8R8A8_UNORM;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return Format::B8G8R8A8_SRGB;
        case VK_FORMAT_UNDEFINED:
            return Format::UNKNOWN;
        default:
            return Format::UNKNOWN;
    }
}

VulkanDynamicRHI::~VulkanDynamicRHI() {
    HUH_ILOG(LogVulkanRHI, "Destroying Vulkan RHI")
}

void VulkanDynamicRHI::Destroy() {
    DynamicRHI::Destroy();
    if (m_instance != VK_NULL_HANDLE) {
        HUH::vkDestroyInstance(m_instance, nullptr);
    }
    delete this;
}

extern "C" HUH_VULKANRHI_API DynamicRHI* DynamicRHICreate() {
    return new VulkanDynamicRHI();
}

}// namespace HUH::RHI
