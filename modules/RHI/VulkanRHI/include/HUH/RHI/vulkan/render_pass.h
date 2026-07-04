#pragma once
#include <HUH/RHI/render_pass.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/vulkan_defines.h>

namespace HUH::RHI {
class VulkanRenderPass : public RenderPass {
public:
    friend class VulkanDevice;
    bool Init() override;
    void Destroy() override;

    operator VkRenderPass() const { return m_renderPass; }

    static VkImageLayout ConvertImageLayout(Layout layout);
    static VkAttachmentLoadOp ConvertLoadOp(LoadOp loadOp);
    static VkAttachmentStoreOp ConvertStoreOp(StoreOp storeOp);

protected:
    explicit VulkanRenderPass(VulkanDevice* device);
    ~VulkanRenderPass() override;

    VulkanDevice* m_device = nullptr;
    VkRenderPass m_renderPass = nullptr;
};
}// namespace HUH::RHI