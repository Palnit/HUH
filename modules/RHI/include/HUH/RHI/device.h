#pragma once

#include <HUH/definitions.h>
#include "HUH/types.h"

namespace HUH::RHI {
class HUH_API Device {
public:
    friend class DynamicRHI;
    enum class Type { Dedicated, Integrated, Other, Unknown };
    struct MemoryStatistics {
        Uint64 DeviceMemory;
        Uint64 OccupiedMemory;
    };

    virtual Device::Type GetType() = 0;
    virtual Device::MemoryStatistics GetMemoryStatistics() = 0;

protected:
    Device() = default;
    virtual ~Device() = default;
};
}// namespace HUH::RHI