#pragma once

#include <vector>
#include <HUH/RHI/types.h>
#include <HUH/RHI/fwd.h>
#include <HUH/definitions.h>
#include <HUH/dynamic_library.h>

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

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    DynamicRHI() = default;
    virtual ~DynamicRHI();
    std::vector<Device*> m_createdDevices;
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary s_RHIImplSharedLibrary;
};

}// namespace RHI

std::string ToString(RHI::Format in_enum);
}// namespace HUH
