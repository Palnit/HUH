#include "HUH/RHI/command_pool.h"

#include <HUH/RHI/dynamic_rhi.h>

#include "HUH/RHI/shader.h"

#include <iostream>
#include <HUH/RHI/rhi_module.h>
#include <HUH/logging.h>
#include <HUH/RHI/device.h>
#include <HUH/RHI/swapchain.h>

namespace HUH {
namespace RHI {
DynamicRHI* DefaultCreate() {
    HUH_LOG(LogRHI, Logging::Level::Warning, "No RHI Library Loaded Try using LoadRHI Or Look at error logs.")
    return nullptr;
}
DynamicRHI::CreateStub* DynamicRHI::Create = &DefaultCreate;

DynamicRHI::~DynamicRHI(){HUH_ILOG(LogRHI, "Destroying RHI")}

DynamicLibrary DynamicRHI::s_RHIImplSharedLibrary;
void DynamicRHI::LoadRHI(const RenderApi api) {
    std::string rhiApiName;
    switch (api) {
        case RenderApi::Vulkan:
            rhiApiName = "HUH-VulkanRHI";
            break;
        case RenderApi::DX12:
            rhiApiName = "HUH-DirectX12RHI";
            break;
        default:
            break;
    }
    if (!s_RHIImplSharedLibrary.Load(DynamicLibrary::DecoratePlatformLibraryName(rhiApiName))) {
        HUH_ELOG(LogRHI, "Cloud not load RHI API: {}", HUH::DynamicLibrary::GetErrorMessage())
    }
    DynamicRHI::Create = s_RHIImplSharedLibrary.GetExport<CreateStub>("DynamicRHICreate");
    if (DynamicRHI::Create == nullptr) {
        DynamicRHI::Create = &DefaultCreate;
    }
}
void DynamicRHI::Destroy() {
    HUH_ILOG(LogRHI, "Destroying Created Devices: ")
    for (Device* device : m_createdDevices) {
        device->Destroy();
        delete device;
    }
}
}// namespace RHI

std::string ToString(RHI::Format in_enum) {
    switch (in_enum) {
        case RHI::Format::UNKNOWN:
            return "Unknown";
        case RHI::Format::R8G8B8A8_UNORM:
            return "R8G8B8A8_UNORM";
        case RHI::Format::R8G8B8A8_SRGB:
            return "R8G8B8A8_SRGB";
        case RHI::Format::B8G8R8A8_UNORM:
            return "B8G8R8A8_UNORM";
        case RHI::Format::B8G8R8A8_SRGB:
            return "B8G8R8A8_SRGB";
        default:
            return "Unknown";
    }
}

}// namespace HUH
