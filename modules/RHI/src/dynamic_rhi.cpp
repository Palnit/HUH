#include <HUH/RHI/dynamic_rhi.h>
#include <iostream>

namespace HUH::RHI {
DynamicRHI* DefaultCreate() {
    std::cout << "No RHI Library Loaded Try using LoadRHI" << std::endl;
    return nullptr;
}
DynamicRHI::CreateStub* DynamicRHI::Create = &DefaultCreate;
DynamicLibrary DynamicRHI::m_RHIImplSharedLibrary;
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
    if (!m_RHIImplSharedLibrary.Load(DynamicLibrary::DecoratePlatformLibraryName(rhiApiName))) {
        //TODO ERROR;
    }
    DynamicRHI::Create = m_RHIImplSharedLibrary.GetExport<CreateStub>("DynamicRHICreate");
}
}// namespace HUH::RHI
