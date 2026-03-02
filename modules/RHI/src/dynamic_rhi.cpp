#include <HUH/RHI/dynamic_rhi.h>

#include "HUH/RHI/shader.h"

#include <iostream>
#include <HUH/RHI/rhi_module.h>
#include <HUH/logging.h>
#include <HUH/RHI/device.h>
#include <HUH/RHI/swapchain.h>

namespace HUH::RHI {
DynamicRHI* DefaultCreate() {
    HUH_LOG(LogRHI, Logging::Level::Warning, "No RHI Library Loaded Try using LoadRHI Or Look at error logs.")
    return nullptr;
}
DynamicRHI::CreateStub* DynamicRHI::Create = &DefaultCreate;
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
    for (Shader* shader : m_createdShaders) {
        shader->Destroy();
        delete shader;
    }
    for (Swapchain* surface : m_createdSwapchains) {
        surface->Destroy();
        delete surface;
    }
    for (Device* device : m_createdDevices) {
        device->Destroy();
        delete device;
    }
}

}// namespace HUH::RHI
