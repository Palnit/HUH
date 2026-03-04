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
    void SetViewPort(Vector2i) override;
    void Init() override;
    void Destroy() override;

protected:
    explicit VulkanCommandBuffer(VulkanPipeline* pipeline);
    ~VulkanCommandBuffer() override;
    VulkanPipeline* m_pipeline;
};
}// namespace HUH::RHI