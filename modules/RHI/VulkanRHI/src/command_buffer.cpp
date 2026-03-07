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

void VulkanCommandBuffer::Init(Queue* queue) {
}

void VulkanCommandBuffer::Destroy() {
}

bool VulkanCommandBuffer::Submit() {
    return false;
}

void VulkanCommandBuffer::Reset() {
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device, VulkanPipeline* pipeline)
    : m_device(device),
      m_pipeline(pipeline) {
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
}

}// namespace HUH::RHI