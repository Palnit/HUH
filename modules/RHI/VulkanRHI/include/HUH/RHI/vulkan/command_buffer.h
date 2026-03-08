#pragma once
#include "pipeline.h"
#include "queue.h"
#include "HUH/Types/tuple.h"

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
    bool Init(Queue* queue) override;
    void Destroy() override;
    bool Submit(Fence<SyncType::GpuToGpu>* wait,
                Fence<SyncType::GpuToGpu>* signal,
                Fence<SyncType::GpuToCpu>* waitSignal) override;
    void Reset() override;

protected:
    explicit VulkanCommandBuffer(VulkanDevice* device, VulkanPipeline* pipeline);
    ~VulkanCommandBuffer() override;

    VulkanDevice* m_device;
    VulkanPipeline* m_pipeline;
    VulkanQueue* m_queue = nullptr;
    VkCommandPool m_commandPool = nullptr;
    VkCommandBuffer m_commandBuffer = nullptr;
    // TODO DO CORRECT FRAME BUFFERS
    VkFramebuffer m_frameBuffer = nullptr;
    VkRect2D m_renderArea{};
};

}// namespace HUH::RHI