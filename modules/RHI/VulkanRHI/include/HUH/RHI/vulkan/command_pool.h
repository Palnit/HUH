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
        void BeginRendering(RenderPass* renderPass, Image* renderTarget) override;
        void EndRendering() override;
        void Reset() override;
        void BindVertexBuffer(Buffer* buffer) override;
        void BindIndexBuffer(Buffer* buffer) override;
        // TODO rethink this with dx12
        void BindUniformBuffers(Buffer* buffer) override;
        void Draw(Uint32 vertexCount, Uint32 instanceCount) override;
        void DrawIndexed(Uint32 indexCount, Uint32 instanceCount) override;
        void BindPipeline(class Pipeline* pipeline) override;
        void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer) override;

    protected:
        VulkanCommandPool* m_parent;
        VulkanPipeline* m_pipeline = nullptr;
        VkCommandBuffer m_commandBuffer;

        VulkanCommandBuffer(VulkanCommandPool* parent, VkCommandBuffer commandBuffer)
            : CommandBuffer(),
              m_parent(parent),
              m_commandBuffer(commandBuffer) {}
        ~VulkanCommandBuffer() override;
    };

    bool Init(Uint32 bufferCount, Queue* queue) override;
    void Destroy() override;

protected:
    explicit VulkanCommandPool(VulkanDevice* device);
    ~VulkanCommandPool() override;

    VulkanDevice* m_device;
    VulkanQueue* m_queue = nullptr;
    VkCommandPool m_commandPool = nullptr;
};

}// namespace HUH::RHI