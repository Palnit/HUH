#include "HUH/RHI/DX12/device.h"
#include "HUH/RHI/DX12/dx12_defines.h"
#include "HUH/logging.h"
#include "HUH/types.h"

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
        HUH_ELOG(LogDX12RHI, "Error during debug interface query")
        return false;
    }
    if (debugInterface->QueryInterface(IID_PPV_ARGS(&m_debugController)) != S_OK) {
        HUH_ELOG(LogDX12RHI, "Error during debug interface query")
        return false;
    }
    m_debugController->EnableDebugLayer();
    m_debugController->SetEnableGPUBasedValidation(true);
    debugInterface->Release();
#endif
    flags |= DXGI_CREATE_FACTORY_DEBUG;
    if (CreateDXGIFactory2(flags, IID_PPV_ARGS(&m_factory)) != S_OK) {
        HUH_ELOG(LogDX12RHI, "Error during factory creation")
        return false;
    }
    HUH_ILOG(LogDX12RHI, "DirectX Rhi Inited Successfully")
    return true;
}
Device* DX12DynamicRHI::GetDevice(size_t index) {
    return nullptr;
}
HUH::Array<Device*> DX12DynamicRHI::GetDevices() {
    IDXGIAdapter1* adapter;
    for (Uint i = 0; DXGI_ERROR_NOT_FOUND != m_factory->EnumAdapters1(i, &adapter); i++) {
        if (D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, _uuidof(ID3D12Device), nullptr) != S_FALSE) {
            continue;
        }
        const auto tmp_device = new DX12Device(adapter);
        // resharper for some reason thinks this never gets a value
        // ReSharper disable once CppDFAConstantConditions
        if (tmp_device->m_adapter == nullptr) {
            continue;
        }
        m_created_devices.Emplace(tmp_device);
    }
    return m_created_devices;
}

Swapchain* DX12DynamicRHI::CreateSwapchain(const Window& window) {
    return nullptr;
}

extern "C" HUH_DIRECTX12RHI_API DynamicRHI* DynamicRHICreate() {
    return new DX12DynamicRHI();
}
}// namespace HUH::RHI