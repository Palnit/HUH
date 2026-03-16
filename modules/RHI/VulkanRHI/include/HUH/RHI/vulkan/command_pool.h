#pragma once
#include "pipeline.h"
#include "queue.h"
#include "HUH/Types/tuple.h"

#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/RHI/command_pool.h>

namespace HUH::RHI {
class VulkanCommandPool : public CommandPool {
public:
    friend class VulkanPipeline;
    friend class VulkanDevice;
    class VulkanCommandBuffer : public CommandBuffer {
    public:
        friend class VulkanCommandPool;
        bool Begin() override;
        void End() override;
        void AddRenderTarget(Image* renderTarget) override;
        bool Submit(Fence* wait, Fence* signal, Fence* waitSignal) override;
        void Reset() override;

    protected:
        VulkanCommandPool* m_parent;
        VkCommandBuffer m_commandBuffer;
        class VulkanImage* m_renderTarget = nullptr;

        VulkanCommandBuffer(VulkanCommandPool* parent, VkCommandBuffer commandBuffer)
            : CommandBuffer(),
              m_parent(parent),
              m_commandBuffer(commandBuffer) {}
        ~VulkanCommandBuffer() override;
    };

    bool Init(Uint32 bufferCount, Queue* queue) override;
    void Destroy() override;

protected:
    explicit VulkanCommandPool(VulkanDevice* device, VulkanPipeline* pipeline);
    ~VulkanCommandPool() override;

    VulkanDevice* m_device;
    VulkanPipeline* m_pipeline;
    VulkanQueue* m_queue = nullptr;
    VkCommandPool m_commandPool = nullptr;
};

}// namespace HUH::RHI