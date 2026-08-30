#pragma once
#include <HUH/RHI/pipeline.h>
#include <HUH/RHI/types.h>
#include <HUH/Types/array.h>
#include <HUH/types.h>

namespace HUH::RHI {
// TODO: A LOT FOR DX12 TO WORK WITH THIS
class RenderPass {
public:
    friend class Device;
    virtual bool Init() = 0;
    virtual void Destroy() = 0;
    // TODO formater
    enum class LoadOp { Load, Clear, DontCare };
    enum class StoreOp { Store, DontCare };
    // TODO: is this needed with dx12 ?
    enum class Layout {
        Unknown,
        General,
        Color,
        DepthStencil,
        DepthStencilReadOnly,
        ShaderReadOnly,
        TransferSrc,
        TransferDst,
        Present,
    };

    struct Attachment {
        Format AttachmentFormat = Format::Unknown;
        /// Color or Depth Component load op
        LoadOp ColorLoadOp = LoadOp::DontCare;
        /// Color or Depth Component store op
        StoreOp ColorStoreOp = StoreOp::DontCare;
        /// Stencil Component load op
        LoadOp StencilLoadOp = LoadOp::DontCare;
        /// Stencil Component store op
        StoreOp StencilStoreOp = StoreOp::DontCare;
        Layout InitialLayout = Layout::Unknown;
        Layout FinalLayout = Layout::Unknown;
        Int64 Index = -1;
    };

    struct SubPass {
        HUH::Array<Attachment> InputAttachments;
        HUH::Array<Attachment> ColorAttachments;
        Attachment DepthAttachments;
    };
    struct Dependency {
        Uint32 SrcSubPassIndex = UnknownSubpass;
        Uint32 DstSubPassIndex = UnknownSubpass;
        Pipeline::Stages SrcStageMask = Pipeline::Stages::Unknown;
        Pipeline::Stages DstStageMask = Pipeline::Stages::Unknown;
        AccessType SrcAccessType = AccessType::Unknown;
        AccessType DstAccessType = AccessType::Unknown;

        static constexpr Uint32 UnknownSubpass = (~0U);
    };

    void AddSubPass(const SubPass& subPass) { m_subPasses.Emplace(subPass); }
    void AddDependency(const Dependency& dependency) { m_dependencies.Emplace(dependency); }

protected:
    RenderPass() = default;
    virtual ~RenderPass() = default;
    HUH::Array<SubPass> m_subPasses;
    HUH::Array<Dependency> m_dependencies;
};
}// namespace HUH::RHI
