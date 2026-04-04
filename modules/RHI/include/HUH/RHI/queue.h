#pragma once

#include <HUH/RHI/fwd.h>
#include <HUH/definitions.h>

namespace HUH::RHI {
class HUH_RHI_API Queue {
    friend class Device;

public:
    enum Type {
        Unknown = 0,
        Graphics = 1 << 0,
        Compute = 1 << 1,
        Transfer = 1 << 2,
        VideoDecode = 1 << 3,
        VideoEncode = 1 << 4
    };

    virtual bool Submit(class CommandPool::CommandBuffer* commandPool,
                        Fence* wait,
                        Fence* signal,
                        Fence* waitSignal) = 0;

protected:
    Queue() = default;
    virtual ~Queue() = default;
    Type m_type = Unknown;
};
}// namespace HUH::RHI
