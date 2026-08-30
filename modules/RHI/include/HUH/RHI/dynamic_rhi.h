#pragma once

#include "HUH/Types/array.h"

#include <HUH/RHI/fwd.h>
#include <HUH/RHI/types.h>
#include <HUH/Types/array.h>
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
    virtual HUH::Array<Device*> GetDevices() = 0;

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    DynamicRHI() = default;
    virtual ~DynamicRHI();
    HUH::Array<Device*> m_createdDevices;
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary s_RHIImplSharedLibrary;
};

}// namespace RHI

std::string ToString(RHI::Format in_enum);
}// namespace HUH
