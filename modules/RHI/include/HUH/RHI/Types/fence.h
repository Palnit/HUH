#pragma once
#include "HUH/types.h"
#include "HUH/RHI/types.h"
#include <HUH/concepts.h>

namespace HUH::RHI {
class Device;

template<SyncType>
class Fence {
    // TODO refactor this maybe not the best solution look into what to do when dx12
    static_assert(false, "Unknown Sync type");
};

template<>
class Fence<SyncType::GpuToCpu> {
public:
    friend class Device;

    virtual bool Wait() = 0;
    virtual bool Wait(Uint64 timeout) = 0;

protected:
    Fence() = default;
    virtual ~Fence() = default;
};

template<>
class Fence<SyncType::GpuToGpu> {
public:
    friend class Device;

protected:
    Fence() = default;
    virtual ~Fence() = default;
};

}// namespace HUH::RHI