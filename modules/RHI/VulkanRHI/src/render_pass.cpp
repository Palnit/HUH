#include <HUH/RHI/fwd.h>
#include <HUH/RHI/vulkan/dynamic_rhi.h>
#include <HUH/RHI/vulkan/fwd.h>
#include <HUH/RHI/vulkan/render_pass.h>

#include <HUH/RHI/vulkan/device.h>
#include "HUH/RHI/vulkan/pipeline.h"
#include "HUH/types.h"
namespace HUH::RHI {

bool VulkanRenderPass::Init() {
    HUH::Array<VkAttachmentDescription2> Attachments;
    HUH::Array<HUH::Array<VkAttachmentReference2>> inputAttachmentsRef;
    HUH::Array<HUH::Array<VkAttachmentReference2>> colorAttachmentsRef;
    HUH::Array<VkAttachmentReference2> depthAttachmentsRef;
    HUH::Array<VkSubpassDescription2> subpasses;
    HUH::Array<VkSubpassDependency2> dependencies;
    for (const auto& subpass : m_subPasses) {
        inputAttachmentsRef.Emplace();
        colorAttachmentsRef.Emplace();
        for (auto [Format, ColorLoadOp, ColorStoreOp, StencilLoadOp, StencilStoreOp, InitialLayout, FinalLayout,
                   Index] : subpass.InputAttachments) {

            VkAttachmentReference2 attachmentRef{
                .sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                .attachment = Index == -1 ? static_cast<Uint32>(Attachments.size()) : static_cast<Uint32>(Index),
                .layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL};

            if (Index == -1) {
                Attachments.Emplace({
                    .sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2,
                    .format = VulkanDynamicRHI::ConvertFormat(Format),
                    // TODO other samples
                    .samples = VK_SAMPLE_COUNT_1_BIT,
                    .loadOp = ConvertLoadOp(ColorLoadOp),
                    .storeOp = ConvertStoreOp(ColorStoreOp),
                    .stencilLoadOp = ConvertLoadOp(StencilLoadOp),
                    .stencilStoreOp = ConvertStoreOp(StencilStoreOp),
                    .initialLayout = ConvertImageLayout(InitialLayout),
                    .finalLayout = ConvertImageLayout(FinalLayout),
                });
            }
            inputAttachmentsRef.back().Emplace(attachmentRef);
        }
        for (auto [Format, ColorLoadOp, ColorStoreOp, StencilLoadOp, StencilStoreOp, InitialLayout, FinalLayout,
                   Index] : subpass.ColorAttachments) {
            VkAttachmentReference2 attachmentRef{
                .sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                .attachment = Index == -1 ? static_cast<Uint32>(Attachments.size()) : static_cast<Uint32>(Index),
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
            if (Index == -1) {
                Attachments.Emplace({
                    .sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR,
                    .format = VulkanDynamicRHI::ConvertFormat(Format),
                    .samples = VK_SAMPLE_COUNT_1_BIT,
                    .loadOp = ConvertLoadOp(ColorLoadOp),
                    .storeOp = ConvertStoreOp(ColorStoreOp),
                    .stencilLoadOp = ConvertLoadOp(StencilLoadOp),
                    .stencilStoreOp = ConvertStoreOp(StencilStoreOp),
                    .initialLayout = ConvertImageLayout(InitialLayout),
                    .finalLayout = ConvertImageLayout(FinalLayout),
                });
            }
            colorAttachmentsRef.back().Emplace(attachmentRef);
        }
        auto [Format, ColorLoadOp, ColorStoreOp, StencilLoadOp, StencilStoreOp, InitialLayout, FinalLayout, Index] =
            subpass.DepthAttachments;
        if (Format != Format::Unknown) {
            depthAttachmentsRef.Emplace(
                {.sType = VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2,
                 .attachment = Index == -1 ? static_cast<Uint32>(Attachments.size()) : static_cast<Uint32>(Index),
                 .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL});
            if (Index == -1) {
                Attachments.Emplace({
                    .sType = VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR,
                    .format = VulkanDynamicRHI::ConvertFormat(Format),
                    .samples = VK_SAMPLE_COUNT_1_BIT,
                    .loadOp = ConvertLoadOp(ColorLoadOp),
                    .storeOp = ConvertStoreOp(ColorStoreOp),
                    .stencilLoadOp = ConvertLoadOp(StencilLoadOp),
                    .stencilStoreOp = ConvertStoreOp(StencilStoreOp),
                    .initialLayout = ConvertImageLayout(InitialLayout),
                    .finalLayout = ConvertImageLayout(FinalLayout),
                });
            }
        }

        // TODO bind point other than graphics
        VkSubpassDescription2 vk_subpass{
            .sType = VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = static_cast<Uint32>(inputAttachmentsRef.back().size()),
            .pInputAttachments = inputAttachmentsRef.back().empty() ? nullptr : inputAttachmentsRef.back().data(),
            .colorAttachmentCount = static_cast<Uint32>(colorAttachmentsRef.back().size()),
            .pColorAttachments = colorAttachmentsRef.back().empty() ? nullptr : colorAttachmentsRef.back().data(),
            .pDepthStencilAttachment = Format == Format::Unknown ? nullptr : &depthAttachmentsRef.back(),
        };
        subpasses.Emplace(vk_subpass);
    }

    for (auto dependency : m_dependencies) {
        VkSubpassDependency2 vk_dependency{
            .sType = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR,
            .srcSubpass = dependency.SrcSubPassIndex,
            .dstSubpass = dependency.DstSubPassIndex,
            .srcStageMask = VulkanPipeline::ConvertToPipelineStage(dependency.SrcStageMask),
            .dstStageMask = VulkanPipeline::ConvertToPipelineStage(dependency.DstStageMask),
            .srcAccessMask = VulkanDynamicRHI::ConvertAccess(dependency.SrcAccessType),
            .dstAccessMask = VulkanDynamicRHI::ConvertAccess(dependency.DstAccessType)};
        dependencies.Emplace(vk_dependency);
    }

    VkRenderPassCreateInfo2 renderPassCreateInfo{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2,
        .pNext = nullptr,
        .attachmentCount = static_cast<Uint32>(Attachments.size()),
        .pAttachments = Attachments.empty() ? nullptr : Attachments.data(),
        .subpassCount = static_cast<Uint32>(subpasses.size()),
        .pSubpasses = subpasses.empty() ? nullptr : subpasses.data(),
        .dependencyCount = static_cast<Uint32>(dependencies.size()),
        .pDependencies = dependencies.empty() ? nullptr : dependencies.data(),
    };
    if (auto err = HUH::vkCreateRenderPass2(*m_device, &renderPassCreateInfo, nullptr, &m_renderPass);
        err != VK_SUCCESS) {
        HUH_ELOG(LogVulkanRHI, "Error Creating Render Pass: {}", err)
        return false;
    }
    return true;
}

void VulkanRenderPass::Destroy() {
    HUH::vkDestroyRenderPass(*m_device, m_renderPass, nullptr);
}

VulkanRenderPass::VulkanRenderPass(VulkanDevice* device) : RenderPass(), m_device(device) {
}

VulkanRenderPass::~VulkanRenderPass() {
    HUH_ILOG(LogVulkanRHI, "Destroying Render Pass");
}

VkImageLayout VulkanRenderPass::ConvertImageLayout(const Layout layout) {
    switch (layout) {
        case Layout::Unknown:
            return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
        case Layout::General:
            return VkImageLayout::VK_IMAGE_LAYOUT_GENERAL;
        case Layout::Color:
            return VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        case Layout::DepthStencil:
            return VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        case Layout::DepthStencilReadOnly:
            return VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
        case Layout::ShaderReadOnly:
            return VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        case Layout::TransferSrc:
            return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        case Layout::TransferDst:
            return VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        case Layout::Present:
            return VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        default:
            return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
    }
}

VkAttachmentLoadOp VulkanRenderPass::ConvertLoadOp(const LoadOp loadOp) {
    switch (loadOp) {
        case LoadOp::Load:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
        case LoadOp::Clear:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
        case LoadOp::DontCare:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        default:
            return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }
}

VkAttachmentStoreOp VulkanRenderPass::ConvertStoreOp(const StoreOp storeOp) {
    switch (storeOp) {
        case StoreOp::Store:
            return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
        case StoreOp::DontCare:
            return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
        default:
            return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
}
}// namespace HUH::RHI