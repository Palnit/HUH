#pragma once

#include "HUH/RHI/render_pass.h"
#include "HUH/RHI/types.h"
#include "HUH/types.h"

#include <HUH/RHI/fwd.h>

namespace HUH::RHI {
class Barrier {
public:
    friend class Device;
    struct Initializer {
        HUH::RHI::Image* Image;
        HUH::RHI::Buffer* Buffer;
        union {
            struct {
                Pipeline::Stages srcStage = Pipeline::Stages::Unknown;
                Pipeline::Stages dstStage = Pipeline::Stages::Unknown;
                AccessType srcAccess = AccessType::Unknown;
                AccessType dstAccess = AccessType::Unknown;
                Int64 srcQueue = -1;
                Int64 dstQueue = -1;
                RenderPass::Layout srcLayout = RenderPass::Layout::Unknown;
                RenderPass::Layout dstLayout = RenderPass::Layout::Unknown;
            } Image;
            struct {
                Pipeline::Stages srcStage = Pipeline::Stages::Unknown;
                Pipeline::Stages dstStage = Pipeline::Stages::Unknown;
                AccessType srcAccess = AccessType::Unknown;
                AccessType dstAccess = AccessType::Unknown;
                Int64 srcQueue = -1;
                Int64 dstQueue = -1;
            } Buffer;
        } Transitions;
    };

    virtual void Init(Initializer&&) = 0;
    virtual void Destroy() { delete this; };

protected:
    Barrier() = default;
    virtual ~Barrier() = default;
};
}// namespace HUH::RHI