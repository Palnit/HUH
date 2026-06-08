#pragma once

#include <HUH/RHI/command_pool.h>
#include <HUH/RHI/fwd.h>
#include <HUH/RHI/pipeline.h>
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
    struct WaitFence {
        Fence* waitFence;
        HUH::RHI::Pipeline::Stages stage;
    };

    virtual bool Submit(CommandPool::CommandBuffer* commandPool, Fence* wait, Fence* signal, Fence* waitSignal) = 0;
    virtual bool Submit(CommandPool::CommandBuffer* commandPool,
                        std::vector<WaitFence> wait,
                        std::vector<Fence*> signal,
                        Fence* waitSignal) = 0;
    virtual bool Submit(CommandPool::CommandBuffer* commandPool) = 0;
    virtual void WaitIdle() = 0;

protected:
    Queue() = default;
    virtual ~Queue() = default;
    Type m_type = Unknown;
};
}// namespace HUH::RHI
