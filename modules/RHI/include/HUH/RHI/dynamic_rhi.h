#pragma once

#include <vector>
#include <HUH/RHI/types.h>
#include <HUH/definitions.h>
#include <HUH/dynamic_library.h>
#include <HUH/RHI/device.h>
#include <HUH/RHI/swapchain.h>

namespace HUH {
class Window;
namespace RHI {

class HUH_RHI_API DynamicRHI {
public:
    static void LoadRHI(RenderApi api);
    using CreateStub = DynamicRHI*();
    static CreateStub* Create;

    virtual bool Init() = 0;
    virtual void Destroy();
    virtual Device* GetDevice(size_t index) = 0;
    virtual std::vector<Device*> GetDevices() = 0;
    virtual Swapchain* CreateSwapchain(const Window& window) = 0;

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    DynamicRHI() = default;
    virtual ~DynamicRHI() = default;
    std::vector<Device*> m_created_devices;
    std::vector<Swapchain*> m_created_surfaces;
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary s_RHIImplSharedLibrary;
};
}// namespace RHI
}// namespace HUH