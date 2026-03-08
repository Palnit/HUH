#include <HUH/RHI/vulkan/queue.h>
#include <HUH/RHI/vulkan/device.h>
namespace HUH::RHI {
VulkanQueue::operator VkQueue() {
    if (m_queue) {
        return m_queue;
    }
    HUH::vkGetDeviceQueue(*m_device, m_familyIndex, m_queueIndex, &m_queue);
    return m_queue;
}

}// namespace HUH::RHI