#pragma once

#include "dx12_defines.h"
#include "HUH/logging.h"

#include <dxgi.h>
#include <dxgi1_6.h>
#include <HUH/RHI/device.h>
namespace HUH::RHI {

class HUH_API DX12Device : public Device {
    friend class DX12DynamicRHI;

public:
    Device::Type GetType() override { return Device::Type::Other; }
    Device::MemoryStatistics GetMemoryStatistics() override { return {}; }
    bool Init(Initializer&& initialization) override {
        m_initialization = initialization;
        return true;
    }
    void Destroy() override { delete this; }

private:
    explicit DX12Device(IDXGIAdapter1* adapter) {
        if (adapter->QueryInterface(IID_PPV_ARGS(&m_adapter)) != S_OK) {
            HUH_ELOG(LogDX12RHI, "Failed to query DXGI adapter4 interface this device will not be used");
        }
    }
    ~DX12Device() override {
        m_adapter->Release();
        HUH_ILOG(LogDX12RHI, "Destroyed")
    }
    IDXGIAdapter4* m_adapter = nullptr;
};
}// namespace HUH::RHI
