#pragma once

#include <HUH/RHI/command_pool.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/Types/tuple.h>

namespace HUH::RHI {
class HUH_VULKANRHI_API VulkanCommandPool : public CommandPool {
public:
    friend class VulkanPipeline;
    friend class VulkanDevice;
    class VulkanCommandBuffer : public CommandBuffer {
    public:
        friend class VulkanCommandPool;
        friend class VulkanQueue;
        bool Begin() override;
        void End() override;
        void AddRenderTarget(Image* renderTarget) override;
        void Reset() override;
        void BindBuffer(Buffer* buffer) override;
        void Draw(Uint32 vertexCount, Uint32 instanceCount) override;

    protected:
        VulkanCommandPool* m_parent;
        VkCommandBuffer m_commandBuffer;
        VulkanImage* m_renderTarget = nullptr;

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