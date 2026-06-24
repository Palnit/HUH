#pragma once

#include <HUH/RHI/command_pool.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>
#include <HUH/Types/tuple.h>
#include <map>

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
        void BeginRendering(RenderPass* renderPass,
                            Image* renderTarget,
                            HUH::Vector2i32 offset,
                            HUH::Vector2u32) override;
        void BeginRendering(RenderPass* renderPass, Image* renderTarget, Image* depthTarget) override;
        void BeginRendering(RenderPass* renderPass,
                            Image* renderTarget,
                            Image* depthTarget,
                            HUH::Vector2i32 offset,
                            HUH::Vector2u32) override;
        void EndRendering() override;
        void Reset() override;
        void BindVertexBuffer(Buffer* buffer, Uint32 binding) override;
        void BindIndexBuffer(Buffer* buffer) override;
        // TODO rethink this with dx12
        void BindUniformBuffers(Buffer* buffer) override;
        void BindSampledImage(Image* image) override;
        void Draw(Uint32 vertexCount, Uint32 instanceCount) override;
        void DrawIndexed(Uint32 indexCount, Uint32 instanceCount) override;
        void BindPipeline(class Pipeline* pipeline) override;
        void CopyBuffer(Buffer* srcBuffer, Buffer* dstBuffer) override;
        void CopyBuffer(Buffer* srcBuffer, Image* dstImage) override;
        void BindBarrier(Barrier* barrier) override;

    protected:
        void BindDescriptorSetWriters();
        VulkanCommandPool* m_parent;
        VulkanPipeline* m_pipeline = nullptr;
        VkCommandBuffer m_commandBuffer;
        std::vector<VkWriteDescriptorSet> m_descriptorWrites;
        std::vector<VkDescriptorSet> m_descriptorSets;
        // TODO this is bad design
        std::vector<VkImageView> m_attachments;
        std::vector<VkImageView> m_attachmentsDepth;
        VkFramebuffer m_frameBuffer = nullptr;
        VkFramebuffer m_frameBufferDepth = nullptr;

        VulkanCommandBuffer(VulkanCommandPool* parent, VkCommandBuffer commandBuffer)
            : CommandBuffer(),
              m_parent(parent),
              m_commandBuffer(commandBuffer) {
            m_attachments.resize(2);
            m_attachmentsDepth.resize(2);
        }
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