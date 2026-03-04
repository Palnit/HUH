#pragma once
#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/pipeline.h>

namespace HUH::RHI {
class VulkanPipeline : public Pipeline {
public:
    bool Init() override;
    void Destroy() override;

protected:
    VulkanPipeline() = default;
    ~VulkanPipeline() override = default;
};
}// namespace HUH::RHI