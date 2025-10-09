#include "HUH/logging.h"
#include "HUH/types.h"
#include "HUH/RHI/DX12/dx12_defines.h"

#include <HUH/RHI/DX12/dynamic_rhi.h>

namespace HUH::RHI {

void DX12DynamicRHI::Destroy() {
#ifdef HUH_DEBUG
    if (m_debugController) {
        m_debugController->Release();
    }
#endif
    if (m_factory) {
        m_factory->Release();
    }
    delete this;
}
bool DX12DynamicRHI::Init() {
    Uint flags = 0;
#ifdef HUH_DEBUG
    ID3D12Debug* debugInterface;
    if (D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)) != S_OK) {
        HUH_LOG(LogDX12RHI, Logging::Level::Log, TEXT("Error during Debug interface query"))
        return false;
    }
    if (debugInterface->QueryInterface(IID_PPV_ARGS(&m_debugController)) != S_OK) {
        HUH_LOG(LogDX12RHI, Logging::Level::Log, TEXT("Error during Debug interface query 2"))
        return false;
    }
    m_debugController->EnableDebugLayer();
    m_debugController->SetEnableGPUBasedValidation(true);
    debugInterface->Release();
#endif
    flags |= DXGI_CREATE_FACTORY_DEBUG;
    if (CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory)) != S_OK) {
        HUH_LOG(LogDX12RHI, Logging::Level::Log, TEXT("Error during factory creation"))
        return false;
    }
    HUH_LOG(LogDX12RHI, Logging::Level::Log, "DirectX Rhi Inited Successfully")
    return true;
}
Device* DX12DynamicRHI::GetDevice(size_t index) {
    return nullptr;
}
std::vector<Device*> DX12DynamicRHI::GetDevices() {
    return std::vector<Device*>();
}

extern "C" HUH_API DynamicRHI* DynamicRHICreate() {
    return new DX12DynamicRHI();
}
}// namespace HUH::RHI