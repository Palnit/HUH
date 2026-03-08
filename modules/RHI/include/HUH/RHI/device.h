#pragma once

#include "types.h"

#include <HUH/RHI/rhi_module.h>
#include <HUH/RHI/queue.h>
#include <HUH/definitions.h>
#include <HUH/types.h>
#include <vector>

namespace HUH {
class Window;
namespace RHI {

template<SyncType>
class Fence;

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
    virtual void Destroy();
    HUH_NODISCARD virtual Device::MemoryStatistics GetMemoryStatistics() = 0;
    virtual Queue* CreateQueue(Queue::Type type) = 0;
    virtual class Shader* CreateShader(void* byteCode, Uint64 size) = 0;
    virtual class Pipeline* CreatePipeline() = 0;
    virtual class CommandBuffer* CreateCommandBuffer(Pipeline* pipeline) = 0;
    virtual class Swapchain* CreateSwapchain(Window& window) = 0;

    virtual Fence<SyncType::GpuToCpu>* CreateFenceGtC() = 0;
    virtual Fence<SyncType::GpuToGpu>* CreateFenceGtG() = 0;

    HUH_NODISCARD Queue* GetQueue(size_t index) const;
    HUH_NODISCARD size_t GetNumberOfQueues() const { return m_queues.size(); };

protected:
    Device() = default;
    virtual ~Device();
    std::vector<Queue*> m_queues;
    std::vector<Shader*> m_createdShaders;
    std::vector<Pipeline*> m_createdPipelines;
    std::vector<CommandBuffer*> m_createdCommandBuffers;
    std::vector<class Swapchain*> m_createdSwapchains;
    std::vector<Fence<SyncType::GpuToCpu>*> m_createdFencesC;
    std::vector<Fence<SyncType::GpuToGpu>*> m_createdFencesG;
};
}// namespace RHI

std::string HUH_RHI_API ToString(RHI::Device::Type inEnum);
std::string HUH_RHI_API ToString(RHI::Device::Vendor inEnum);

}// namespace HUH

ENUM_FORMATER(HUH::RHI::Device::Type)
ENUM_FORMATER(HUH::RHI::Device::Vendor)
