#pragma once

#include <HUH/RHI/Types/buffer.h>
#include <HUH/RHI/fwd.h>
#include <HUH/RHI/queue.h>
#include <HUH/definitions.h>
#include <HUH/types.h>
#include <vector>

namespace HUH {
class Window;
namespace RHI {
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
#ifdef HUH_USE_CUDA
        Uint8* DeviceUUID = nullptr;
        Uint32 DeviceUUIDSize = 0;
#endif
    };

    // TODO make it so this can be used with the shared memory library when i make thet instead of only cuda

    DeviceInformation Information;

    virtual bool Init() = 0;
    virtual void Destroy();
    HUH_NODISCARD virtual Device::MemoryStatistics GetMemoryStatistics() = 0;
    virtual Queue* RequestQueue(Queue::Type type) = 0;
    virtual Shader* CreateShader(void* byteCode, Uint64 size) = 0;
    virtual Pipeline* CreatePipeline() = 0;
    virtual CommandPool* CreateCommandPool() = 0;
    virtual Swapchain* CreateSwapchain(Window& window) = 0;
    virtual MemoryAllocator* CreateMemoryAllocator() = 0;
    virtual Fence* CreateFence() = 0;
    virtual Barrier* CreateBarrier() = 0;
    virtual std::vector<Fence*> CreateFence(Uint32 num) = 0;
    virtual RenderPass* CreateRenderPass() = 0;

    HUH_NODISCARD Queue* GetQueue(size_t index) const;
    HUH_NODISCARD size_t GetNumberOfQueues() const { return m_queues.size(); };

protected:
    Device() = default;
    virtual ~Device();
    std::vector<Queue*> m_queues;
    std::vector<Shader*> m_createdShaders;
    std::vector<Pipeline*> m_createdPipelines;
    std::vector<CommandPool*> m_createdCommandBuffers;
    std::vector<Swapchain*> m_createdSwapchains;
    std::vector<Fence*> m_createdFences;
    std::vector<MemoryAllocator*> m_createdMemoryAllocators;
    std::vector<RenderPass*> m_createdRenderPasses;
};
}// namespace RHI

std::string HUH_RHI_API ToString(RHI::Device::Type inEnum);
std::string HUH_RHI_API ToString(RHI::Device::Vendor inEnum);

}// namespace HUH

HUH_ENUM_FORMATER(HUH::RHI::Device::Type)
HUH_ENUM_FORMATER(HUH::RHI::Device::Vendor)
