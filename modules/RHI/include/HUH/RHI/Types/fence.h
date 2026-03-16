#pragma once
#include "HUH/types.h"
#include "HUH/RHI/types.h"
#include <HUH/concepts.h>

namespace HUH::RHI {
class Device;

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