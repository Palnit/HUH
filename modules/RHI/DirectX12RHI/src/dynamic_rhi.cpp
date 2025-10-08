#include "HUH/logging.h"
#include "HUH/RHI/DX12/dx12_defines.h"

#include <HUH/RHI/DX12/dynamic_rhi.h>

namespace HUH::RHI {

void DX12DynamicRHI::Destroy() {
    delete this;
}
bool DX12DynamicRHI::Init() {
    HUH_LOG(LogDX12RHI, Logging::Level::Log, "DirectX Rhi Inited Successfully") return true;
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