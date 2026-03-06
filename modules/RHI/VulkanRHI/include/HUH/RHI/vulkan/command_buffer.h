#pragma once
#include "pipeline.h"

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/command_buffer.h>

namespace HUH::RHI {
class VulkanCommandBuffer : public CommandBuffer {
public:
    friend class VulkanPipeline;
    friend class VulkanDynamicRHI;

    bool Begin() override;
    void End() override;
    void AddRenderTarget(Image* renderTarget) override;
    void Init(Device* device, Queue* queue) override;
    void Destroy() override;

protected:
    explicit VulkanCommandBuffer(VulkanPipeline* pipeline);
    ~VulkanCommandBuffer() override;
    VulkanPipeline* m_pipeline;
    VulkanDevice* m_device = nullptr;
    VkCommandPool m_commandPool = nullptr;
};

}// namespace HUH::RHI