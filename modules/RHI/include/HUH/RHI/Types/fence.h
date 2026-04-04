#pragma once
#include <HUH/types.h>
#include <HUH/RHI/types.h>
#include <HUH/concepts.h>
#include <HUH/RHI/fwd.h>

namespace HUH::RHI {
class Fence {
public:
    friend class Device;

    virtual bool Wait() = 0;
    virtual bool Wait(Uint64 timeout) = 0;
    virtual void Reset() = 0;

protected:
    Fence() = default;
    virtual ~Fence() = default;
};

}// namespace HUH::RHI