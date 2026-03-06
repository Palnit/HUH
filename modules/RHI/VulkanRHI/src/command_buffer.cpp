#include <HUH/Math/vector.h>
#include <HUH/RHI/vulkan/command_buffer.h>
#include <HUH/RHI/vulkan/device.h>

namespace HUH::RHI {

bool VulkanCommandBuffer::Begin() {
    return false;
}

void VulkanCommandBuffer::End() {
}

void VulkanCommandBuffer::AddRenderTarget(Image* renderTarget) {
}

void VulkanCommandBuffer::Init(Device* device, Queue* queue) {
    m_device = dynamic_cast<VulkanDevice*>(device);
}

void VulkanCommandBuffer::Destroy() {
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanPipeline* pipeline) : m_pipeline(pipeline) {
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
}

}// namespace HUH::RHI