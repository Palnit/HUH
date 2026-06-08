#pragma once

#include <HUH/RHI/Types/buffer.h>

#include <HUH/RHI/fwd.h>
#include <HUH/RHI/types.h>

#include <HUH/concepts.h>
#include <HUH/types.h>

namespace HUH::RHI {
class Fence {
public:
    friend class Device;

    virtual bool Wait() = 0;
    virtual bool Wait(Uint64 timeout) = 0;
    virtual void Reset() = 0;
#ifdef HUH_USE_CUDA
    virtual Buffer::SharedMemoryInfo GetSharedMemory() = 0;
#endif

protected:
    Fence() = default;
    virtual ~Fence() = default;
};

}// namespace HUH::RHI