#include <HUH/RHI/vulkan/device.h>
namespace HUH::RHI {
Device::MemoryStatistics VulkanDevice::GetMemoryStatistics() {
    return {};
}
VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice) : m_physicalDevice(physicalDevice) {
    VkPhysicalDeviceProperties properties;
    HUH::vkGetPhysicalDeviceProperties(m_physicalDevice, &properties);
    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            m_type = Type::Other;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            m_type = Type::Software;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            m_type = Type::Cpu;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            m_type = Type::Integrated;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            m_type = Type::Dedicated;
            break;
        default:;
    }
}
}// namespace HUH::RHI