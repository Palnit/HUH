#pragma once

#include <unordered_map>
#include <HUH/definitions.h>
#include <HUH/RHI/device.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanDevice final : public Device {
    friend class VulkanDynamicRHI;
    friend class VulkanSwapchain;
    friend class VulkanShader;
    friend class VulkanImage;
    friend class VulkanPipeline;

public:
    struct VulkanDeviceProperties {
        VkPhysicalDeviceProperties2KHR properties_1_0{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR};
        VkPhysicalDeviceVulkan11Properties properties_1_1{.sType =
                                                              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES};
        VkPhysicalDeviceVulkan12Properties properties_1_2{.sType =
                                                              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES};
        VkPhysicalDeviceVulkan13Properties properties_1_3{.sType =
                                                              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES};
        VkPhysicalDeviceVulkan14Properties properties_1_4{.sType =
                                                              VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_PROPERTIES};
        VulkanDeviceProperties() {
            // TODO Vulkan versioning
            properties_1_0.pNext = &properties_1_1;
            properties_1_1.pNext = &properties_1_2;
            properties_1_2.pNext = &properties_1_3;
            properties_1_3.pNext = &properties_1_4;
        }
    };

    struct VulkanDeviceFeatures {
        VkPhysicalDeviceFeatures2KHR features_1_0{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR};
        VkPhysicalDeviceVulkan11Features features_1_1{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES};
        VkPhysicalDeviceVulkan12Features features_1_2{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES};
        VkPhysicalDeviceVulkan13Features features_1_3{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
        VkPhysicalDeviceVulkan14Features features_1_4{.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES};
        VulkanDeviceFeatures() {
            features_1_0.pNext = &features_1_1;
            features_1_1.pNext = &features_1_2;
            features_1_2.pNext = &features_1_3;
            features_1_3.pNext = &features_1_4;
        }
    };

    VulkanDeviceProperties Properties;
    VulkanDeviceFeatures Features;

    HUH_NODISCARD Device::MemoryStatistics GetMemoryStatistics() override;
    bool Init() override;
    void Destroy() override;
    Queue* CreateQueue(Queue::Type type) override;

    void QueryVulkanPropertiesAndFeatures();

    Shader* CreateShader(void* byteCode, Uint64 size) override;
    Pipeline* CreatePipeline() override;
    CommandBuffer* CreateCommandBuffer(Pipeline* pipeline) override;
    Swapchain* CreateSwapchain(Window& window) override;

    // ReSharper disable once CppNonExplicitConversionOperator
    operator VkDevice() const { return m_device; }

protected:
    explicit VulkanDevice(class VulkanDynamicRHI* parent, VkPhysicalDevice physicalDevice);
    ~VulkanDevice() override;

    VulkanDynamicRHI* m_parent;
    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice m_device = nullptr;
    std::vector<VkQueueFamilyProperties> m_queueFamilies;
    std::unordered_map<size_t, size_t> m_familyQueueCount;
};

}// namespace HUH::RHI
