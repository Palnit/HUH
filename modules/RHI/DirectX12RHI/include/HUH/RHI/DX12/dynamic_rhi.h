#pragma once

#include <HUH/RHI/dynamic_rhi.h>
#include <dxgi.h>
#include <dxgi1_6.h>

#ifdef HUH_DEBUG
#include <d3d12sdklayers.h>
#endif

namespace HUH::RHI {
extern "C" HUH_DIRECTX12RHI_API HUH::RHI::DynamicRHI* DynamicRHICreate();

class HUH_DIRECTX12RHI_API DX12DynamicRHI final : public RHI::DynamicRHI {
public:
    void Destroy() override;
    bool Init() override;
    Device* GetDevice(size_t index) override;
    std::vector<Device*> GetDevices() override;
    Surface* CreateSurface(const Window& window) override;

    friend HUH::RHI::DynamicRHI* DynamicRHICreate();

protected:
    DX12DynamicRHI() = default;
    ~DX12DynamicRHI() override = default;

    IDXGIFactory7* m_factory = nullptr;
#ifdef HUH_DEBUG
    ID3D12Debug1* m_debugController = nullptr;
#endif
};
}// namespace HUH::RHI
