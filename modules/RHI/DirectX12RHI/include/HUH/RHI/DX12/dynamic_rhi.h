#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <dxgi.h>
#include <dxgi1_6.h>

#ifdef HUH_DEBUG
#include <dxgidebug.h>
#endif

namespace HUH::RHI {
extern "C" HUH_API HUH::RHI::DynamicRHI* DynamicRHICreate();

class HUH_API DX12DynamicRHI final : public RHI::DynamicRHI {
public:
    void Destroy() override;
    bool Init() override;
    Device* GetDevice(size_t index) override;
    std::vector<Device*> GetDevices() override;

    friend HUH::RHI::DynamicRHI* DynamicRHICreate();

protected:
    DX12DynamicRHI() = default;
    ~DX12DynamicRHI() override = default;
    IDXGIFactory7* factory = nullptr;
};
}// namespace HUH::RHI
