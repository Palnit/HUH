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
                AccessType srcAccess;
                AccessType dstAccess;
                Int64 srcQueue;
                Int64 dstQueue;
                RenderPass::Layout srcLayout;
                RenderPass::Layout dstLayout;
            } Image;
            struct {
                AccessType srcAccess;
                AccessType dstAccess;
                Int64 srcQueue;
                Int64 dstQueue;
            } Buffer;
        } Transitions;
    };

    virtual void Init(Initializer&&) = 0;
    virtual void Destroy() = 0;

protected:
    Barrier() = default;
    virtual ~Barrier() = default;
};
}// namespace HUH::RHI