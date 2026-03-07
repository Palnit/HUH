#pragma once

#include <vector>
#include <HUH/RHI/types.h>
#include <HUH/definitions.h>
#include <HUH/dynamic_library.h>
#include <HUH/RHI/device.h>
#include <HUH/RHI/pipeline.h>
#include <HUH/RHI/command_buffer.h>

namespace HUH {
class Window;
namespace RHI {

enum class Format {
    UNKNOWN,
    R8G8B8A8_UNORM,
    R8G8B8A8_SRGB,
    B8G8R8A8_UNORM,
    B8G8R8A8_SRGB,
};

class HUH_RHI_API DynamicRHI {
public:
    static void LoadRHI(RenderApi api);
    using CreateStub = DynamicRHI*();
    static CreateStub* Create;

    virtual bool Init() = 0;
    virtual void Destroy();
    virtual Device* GetDevice(size_t index) = 0;
    virtual std::vector<Device*> GetDevices() = 0;

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    DynamicRHI() = default;
    virtual ~DynamicRHI() = default;
    std::vector<Device*> m_createdDevices;
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary s_RHIImplSharedLibrary;
};
}// namespace RHI
}// namespace HUH