#pragma once

#include <vector>
#include <HUH/RHI/types.h>
#include <HUH/definitions.h>
#include <HUH/dynamic_library.h>


namespace HUH::RHI {

enum class DeviceType {
    Dedicated,
    Integrated,
};

class HUH_API DynamicRHI {
public:
    friend class Device;

    static void LoadRHI(RenderApi api);
    using CreateStub = DynamicRHI*();
    static CreateStub* Create;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;
    virtual Device* GetDevice(size_t index) = 0;
    virtual std::vector<Device*> GetDevices() = 0;

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    std::vector<Device*> m_created_devices;
    DynamicRHI() = default;
    virtual ~DynamicRHI();
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary s_RHIImplSharedLibrary;
};
}// namespace HUH::RHI