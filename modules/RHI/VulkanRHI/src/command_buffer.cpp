#include <HUH/Math/vector.h>
#include <HUH/RHI/vulkan/command_buffer.h>

namespace HUH::RHI {

bool VulkanCommandBuffer::Begin() {
    return false;
}

void VulkanCommandBuffer::End() {
}

void VulkanCommandBuffer::AddRenderTarget(Image* renderTarget) {
}

void VulkanCommandBuffer::SetViewPort(Vector2i) {
}

void VulkanCommandBuffer::Init() {
}

void VulkanCommandBuffer::Destroy() {
}

VulkanCommandBuffer::VulkanCommandBuffer(VulkanPipeline* pipeline) : m_pipeline(pipeline) {
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
}

}// namespace HUH::RHI