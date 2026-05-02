#pragma once

#include <HUH/RHI/device.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/definitions.h>
#include <unordered_map>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanDevice final : public Device {
    friend class VulkanDynamicRHI;
    friend class VulkanSwapchain;
    friend class VulkanShader;
    friend class VulkanImage;
    friend class VulkanPipeline;
    friend class VulkanCommandPool;
    friend class VulkanMemoryAllocator;
    friend class VulkanRenderPass;

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
    Queue* RequestQueue(Queue::Type type) override;

    void QueryVulkanPropertiesAndFeatures();

    Shader* CreateShader(void* byteCode, Uint64 size) override;
    Pipeline* CreatePipeline() override;
    CommandPool* CreateCommandPool() override;
    Swapchain* CreateSwapchain(Window& window) override;

    // ReSharper disable once CppNonExplicitConversionOperator
    operator VkDevice() const { return m_device; }

    Fence* CreateFence() override;
    std::vector<Fence*> CreateFence(Uint32 num) override;

    MemoryAllocator* CreateMemoryAllocator() override;
    Buffer* CreateBuffer(Buffer::Type type, Uint64 Size) override;
    RenderPass* CreateRenderPass() override;

protected:
    explicit VulkanDevice(VulkanDynamicRHI* parent, VkPhysicalDevice physicalDevice);
    ~VulkanDevice() override;

    VulkanDynamicRHI* m_parent;
    VkPhysicalDevice m_physicalDevice = nullptr;
    VkDevice m_device = nullptr;
    // TODO: replace with 2
    VkPhysicalDeviceMemoryProperties m_memoryProperties;
    std::vector<VkQueueFamilyProperties> m_queueFamilies;
    std::unordered_map<size_t, size_t> m_familyQueueCount;
};

}// namespace HUH::RHI
