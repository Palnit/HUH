#include <HUH/string_operations.h>
#include <HUH/RHI/vulkan/queue.h>
#include <HUH/VulkanHelpers/string_converters.h>
#include <HUH/RHI/vulkan/device.h>
#include <HUH/enum_helper.h>

namespace HUH::RHI {
Device::MemoryStatistics VulkanDevice::GetMemoryStatistics() {
    return {};
}
bool VulkanDevice::Init() {
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    float queuePriority = 1.0f;
    for (auto [index, size] : m_familyQueueCount) {
        if (size == 0) {
            continue;
        }
        VkDeviceQueueCreateInfo queueInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                                          .queueFamilyIndex = static_cast<Uint32>(index),
                                          .queueCount = static_cast<Uint32>(size),
                                          .pQueuePriorities = &queuePriority};
        queueInfos.emplace_back(queueInfo);
    }

    Uint32 extensionCount;
    HUH::vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    HUH::vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, availableExtensions.data());

#if HUH_DEBUG
    HUH_LOG(LogVulkanRHI, Logging::Log, "Available Instance extensions:")
    for (size_t i = 0; i < availableExtensions.size(); i++) {
        HUH_LOG(LogVulkanRHI, Logging::Log, "{}.\t{}", i + 1, availableExtensions[i].extensionName)
    }
#endif

    // TODO MAKE THIS A VECTOR FROM DEFAULT
    const std::string requiredExtensionsString = HUH_REQUIRED_DEVICE_EXTENSIONS;
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

    VkDeviceCreateInfo deviceInfo{.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                                  .pNext = &Features.features_1_0,
                                  .queueCreateInfoCount = static_cast<Uint32>(queueInfos.size()),
                                  .pQueueCreateInfos = queueInfos.data(),
                                  .enabledExtensionCount = static_cast<Uint32>(requiredExtensions.size()),
                                  .ppEnabledExtensionNames = requiredExtensions.data()};
    if (auto err = HUH::vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device); err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Device Creation Error: {}", HUH::ToString(err))
        return false;
    }
    HUH_ILOG(LogVulkanRHI, "Device creation is successful for device named: {}",
             Properties.properties_1_0.properties.deviceName)
    return true;
}
void VulkanDevice::Destroy() {
    if (m_device) {
        HUH::vkDestroyDevice(m_device, nullptr);
    }
}
Queue* VulkanDevice::CreateQueue(Queue::Type type) {
    if (m_device) {
        HUH_ELOG(LogVulkanRHI, "Trying to create Vulkan queue after device init is not allowed");
        return nullptr;
    }
    if (m_queueFamilies.empty()) {

        Uint32 queueFamilyIndex = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyIndex, nullptr);
        m_queueFamilies.resize(queueFamilyIndex);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyIndex, m_queueFamilies.data());
#ifdef HUH_DEBUG
        HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Available Queue Families:")
        for (size_t i = 0; i < m_queueFamilies.size(); i++) {
            std::string flags;
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_GRAPHICS_BIT)) {
                flags += "Graphics | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_COMPUTE_BIT)) {
                flags += "Compute | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_TRANSFER_BIT)) {
                flags += "Transfer | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_SPARSE_BINDING_BIT)) {
                flags += "Sparse Binding | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_PROTECTED_BIT)) {
                flags += "Protected | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_VIDEO_DECODE_BIT_KHR)) {
                flags += "Video Decode KHR | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_VIDEO_ENCODE_BIT_KHR)) {
                flags += "Video Encode KHR | ";
            }
            if (CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_OPTICAL_FLOW_BIT_NV)) {
                flags += "Optical Flow NV | ";
            }
            flags = flags.substr(0, flags.size() - 3);
            HUH_ILOG(LogVulkanRHI, "{}. Flags: {}, Queue Count: {}", i, flags, m_queueFamilies[i].queueCount)
        }
#endif
    }

    size_t match_index = 0;
    bool found = false;
    for (size_t i = 0; i < m_queueFamilies.size(); i++) {
        bool match = true;
        if (CheckFlag(type, Queue::Graphics)
            && !CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_GRAPHICS_BIT)) {
            match = false;
        }
        if (CheckFlag(type, Queue::Compute)
            && !CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_COMPUTE_BIT)) {
            match = false;
        }
        if (CheckFlag(type, Queue::Transfer)
            && !CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_TRANSFER_BIT)) {
            match = false;
        }
        if (CheckFlag(type, Queue::VideoDecode)
            && !CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_VIDEO_DECODE_BIT_KHR)) {
            match = false;
        }
        if (CheckFlag(type, Queue::VideoEncode)
            && !CheckFlag(static_cast<VkQueueFlagBits>(m_queueFamilies[i].queueFlags), VK_QUEUE_VIDEO_ENCODE_BIT_KHR)) {
            match = false;
        }
        if (match && m_familyQueueCount[i] < m_queueFamilies[i].queueCount) {
            if (found) {
                if (HUH_POP_COUNT32(m_queueFamilies[i].queueFlags)
                    < HUH_POP_COUNT32(m_queueFamilies[match_index].queueFlags)) {
                    match_index = i;
                }
            } else {
                match_index = i;
                found = true;
            }
        }
    }
    if (!found) {
        HUH_WLOG(LogVulkanRHI, "Couldn't Create Queue")
        return nullptr;
    }

    m_queues.emplace_back(
        new VulkanQueue(match_index, m_familyQueueCount[match_index]++, m_queueFamilies[match_index]));
    return m_queues.back();
}
void VulkanDevice::QueryVulkanPropertiesAndFeatures() {
    HUH::vkGetPhysicalDeviceProperties2KHR(m_physicalDevice, &Properties.properties_1_0);
    HUH::vkGetPhysicalDeviceFeatures2KHR(m_physicalDevice, &Features.features_1_0);
}

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice) : m_physicalDevice(physicalDevice) {
    QueryVulkanPropertiesAndFeatures();
    m_name = Properties.properties_1_0.properties.deviceName;
    switch (Properties.properties_1_0.properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            m_type = Type::Other;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            m_type = Type::Virtual;
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
VulkanDevice::~VulkanDevice() {
    HUH_LOG(LogVulkanRHI, Logging::Level::Log, "Vulkan Device Named: {} Successfully Destroyed", m_name)
}
}// namespace HUH::RHI