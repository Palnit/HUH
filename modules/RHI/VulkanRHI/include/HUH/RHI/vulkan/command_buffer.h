#pragma once
#include "pipeline.h"

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/command_buffer.h>

namespace HUH::RHI {
class VulkanCommandBuffer : public CommandBuffer {
public:
    friend class VulkanPipeline;
    friend class VulkanDevice;

    bool Begin() override;
    void End() override;
    void AddRenderTarget(Image* renderTarget) override;
    void Init(Queue* queue) override;
    void Destroy() override;
    bool Submit() override;
    void Reset() override;

protected:
    explicit VulkanCommandBuffer(VulkanDevice* device, VulkanPipeline* pipeline);
    ~VulkanCommandBuffer() override;

    VulkanDevice* m_device;
    VulkanPipeline* m_pipeline;
    VkCommandPool m_commandPool = nullptr;
};

}// namespace HUH::RHI