#pragma once

#include <HUH/definitions.h>
#include "HUH/types.h"

namespace HUH::RHI {
class HUH_API Device {
public:
    friend class DynamicRHI;
    enum Type { Dedicated, Integrated, Cpu, Software, Other, Unknown };
    struct MemoryStatistics {
        Uint64 DeviceMemory;
        Uint64 OccupiedMemory;
    };

    struct Initializer {
        Uint64 NumberOfQueues;
    };

    virtual bool Init(Initializer&& initialization) = 0;
    virtual void Destroy() = 0;
    virtual Device::Type GetType() = 0;
    virtual Device::MemoryStatistics GetMemoryStatistics() = 0;

protected:
    Device() = default;
    virtual ~Device() = default;
    Type m_type = Type::Unknown;
    Initializer m_initialization = {};
};
}// namespace HUH::RHI