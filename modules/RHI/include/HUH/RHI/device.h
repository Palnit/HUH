#pragma once

#include "rhi_module.h"

#include <HUH/RHI/queue.h>
#include <HUH/definitions.h>
#include <HUH/types.h>
#include <vector>

namespace HUH::RHI {
class Queue;
class HUH_RHI_API Device {
public:
    friend class DynamicRHI;
    enum Type { Dedicated, Integrated, Cpu, Virtual, Other, Unknown };

    struct MemoryStatistics {
        Uint64 DeviceMemory;
        Uint64 OccupiedMemory;
    };

    virtual bool Init() = 0;
    virtual void Destroy() = 0;
    HUH_NODISCARD virtual Device::Type GetType() = 0;
    HUH_NODISCARD virtual std::string GetName() = 0;
    HUH_NODISCARD virtual Device::MemoryStatistics GetMemoryStatistics() = 0;
    virtual Queue* CreateQueue(Queue::Type type) = 0;

    HUH_NODISCARD Queue* GetQueue(size_t index) const;

protected:
    Device() = default;
    virtual ~Device();
    std::string m_name;
    Type m_type = Type::Unknown;
    std::vector<Queue*> m_queues;
};
}// namespace HUH::RHI