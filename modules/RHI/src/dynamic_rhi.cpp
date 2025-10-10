#include <HUH/RHI/dynamic_rhi.h>
#include <iostream>
#include <HUH/RHI/rhi_module.h>
#include <HUH/logging.h>
#include <HUH/RHI/device.h>

namespace HUH::RHI {
DynamicRHI* DefaultCreate() {
    HUH_LOG(LogRHI, Logging::Level::Warning, "No RHI Library Loaded Try using LoadRHI")
    return nullptr;
}
DynamicRHI::CreateStub* DynamicRHI::Create = &DefaultCreate;
DynamicLibrary DynamicRHI::s_RHIImplSharedLibrary;
void DynamicRHI::LoadRHI(RenderApi api) {
    std::string rhiApiName;
    switch (api) {
        case RenderApi::OpenGl:
            break;
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
        // TODO ERROR;
        HUH_ELOG(LogRHI, "Cloud not load RHI API: {}", HUH::DynamicLibrary::GetErrorMessage())
    }
    DynamicRHI::Create = s_RHIImplSharedLibrary.GetExport<CreateStub>("DynamicRHICreate");
    if (DynamicRHI::Create == nullptr) {
        // TODO NORMAL ERROR;
        DynamicRHI::Create = &DefaultCreate;
    }
}
DynamicRHI::~DynamicRHI() {
    HUH_ILOG(LogRHI, "Destroying Created Devices")
    for (const Device* device : m_created_devices) {
        delete device;
    }
}
}// namespace HUH::RHI
