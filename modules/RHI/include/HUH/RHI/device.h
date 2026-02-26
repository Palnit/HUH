#pragma once

#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/queue.h>
#include <HUH/definitions.h>
#include <HUH/types.h>
#include <vector>

namespace HUH {
namespace RHI {
class Queue;
class HUH_RHI_API Device {
public:
    friend class DynamicRHI;
    friend class Swapchain;
    enum class Type { Dedicated, Integrated, Cpu, Virtual, Other, Unknown };
    enum class Vendor { Nvidia, Amd, Arm, Qualcomm, Intel, Unknown };

    struct MemoryStatistics {
        Uint64 DeviceMemory;
        Uint64 OccupiedMemory;
    };

    struct DeviceInformation {
        std::string name = "Unknown";
        Vendor vendor = Vendor::Unknown;
        Type type = Type::Unknown;
    };

    DeviceInformation Information;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;
    HUH_NODISCARD virtual Device::MemoryStatistics GetMemoryStatistics() = 0;
    virtual Queue* CreateQueue(Queue::Type type) = 0;

    HUH_NODISCARD Queue* GetQueue(size_t index) const;
    HUH_NODISCARD size_t GetNumberOfQueues() const { return m_queues.size(); };

protected:
    Device() = default;
    virtual ~Device();
    std::vector<Queue*> m_queues;
};
}// namespace RHI

std::string HUH_RHI_API ToString(RHI::Device::Type type);
std::string HUH_RHI_API ToString(RHI::Device::Vendor vendor);

}// namespace HUH