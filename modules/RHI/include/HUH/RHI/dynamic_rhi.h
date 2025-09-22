#pragma once

#include <HUH/RHI/types.h>
#include <HUH/definitions.h>
#include <HUH/dynamic_library.h>

namespace HUH::RHI {

class HUH_API DynamicRHI {
public:
    static void LoadRHI(RenderApi api);
    using CreateStub = DynamicRHI*();
    static CreateStub* Create;

    virtual bool Init() = 0;
    virtual void Destroy() = 0;

    DynamicRHI(const DynamicRHI&) = delete;
    DynamicRHI& operator=(const DynamicRHI&) = delete;
    DynamicRHI(DynamicRHI&&) = delete;
    DynamicRHI& operator=(DynamicRHI&&) = delete;

protected:
    DynamicRHI() = default;
    virtual ~DynamicRHI() = default;
    RenderApi m_renderApi = RenderApi::Unknown;
    static DynamicLibrary m_RHIImplSharedLibrary;
};
}// namespace HUH::RHI